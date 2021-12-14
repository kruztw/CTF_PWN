// g_text  0x1300c

void main()
{ 
    init();
    for(int i = 0; i<3; i++)
    while (i < 3) {
        printf("Text: ");
        read(&g_text[0x40], 0x200);
        if(g_text[0x40] == '\0') break;
        printf("With salt? [y/N] ");
        op = getchar();
        getchar();
        if ((op == 'Y') || (op == 'y')) {
            calc_md5(g_text[0x40], g_text, 1);
            printf("MD5 (salted): %s\n",g_text);
            printf("Salt: \\x%02x\\x%02x\\x%02x\\x%02x\n", g_text + 0x240, (g_text + 0x240) + 1, (g_text + 0x240) + 2, (g_text + 0x240) + 3);
        }
        else {
            calc_md5(g_text[0x40], g_text, 0);
            printf("MD5: %s\n",g_text);
        }
        i = i + 1;
    }

    fini();
    return 0;
}


void init()
{
    undefined auStack540 [12];
    undefined local_210 [512];
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);

    pcStack556 = (char *)0x0;
    pcStack556 = (char *)FUN_00010810();
    FUN_00010790();
    pcStack556 = (char *)0x244;
    g_text = FUN_00010818();
    *(int *)(g_text + 0x240) = g_text + 0x3c;
    puStack552 = &DAT_00011048;
    pcStack556 = "banner.txt";
    DAT_00013010 = FUN_00010800();
    ppop = (char **)auStack540;
    if (DAT_00013010 == 0) {
        pcStack556 = "fopen";
        FUN_000107d8();
        pcStack556 = (char *)0x1;
        exit();
        ppop = &pcStack556;
    }
    *(undefined4 *)((undefined *)ppop-8) = 0x200;
    *(undefined4 *)((undefined *)ppop-0xc) = 0;
    *(undefined **)((undefined *)ppop-0x10) = local_210;
    *(undefined4 *)((undefined *)ppop-0x14) = 0x10d2e;
    FUN_000107c0();
    *(int *)((undefined *)ppop + -4) = DAT_00013010;
    *(undefined4 *)((undefined *)ppop + -8) = 0x200;
    *(undefined4 *)((undefined *)ppop + -0xc) = 1;
    *(undefined **)((undefined *)ppop + -0x10) = local_210;
    *(undefined4 *)((undefined *)ppop + -0x14) = 0x10d4d;
    FUN_00010830();
    *(undefined **)((undefined *)ppop + -0xc) = local_210;
    *(undefined4 *)((undefined *)ppop + -0x10) = 0x1105b;
    *(undefined4 *)((undefined *)ppop + -0x14) = 0x10d66;
    printf();
}


void fini(void)

{
  FUN_000107e8(DAT_00013010);
  fclose(g_text);
  return;
}


void calc_md5(int plain, int cipher,int is_salt)
{
    int aiStack224 [5];
    byte *apbStack204 [12];
    undefined auStack156 [8];
    char local_7c [92];
    byte local_20 [16];
    ppcVar5 = (char **)auStack156;
    apbStack204[11] = 0x109d0;
    plain_ptr = plain;
    cipher_ptr = cipher;
    if (is_salt == 0)
        plain_ptr = plain;
    else {
        plain_ptr = plain-4;
        for(int i = 0; i<4; i++){
            puVar6 = DAT_0001300c[0x240+i];
            apbStack204[11] = 0x10a35;
            uVar1 = FUN_00010828();
            *puVar6 = uVar1;
            if (DAT_0001300c[0x240+i] == '\0') {
                pcVar2 = DAT_0001300c[0x240+i];
                *pcVar2 = *pcVar2 + '\x01';
            }
        }
    }

    apbStack204[8] = local_7c;
    iVar3 = FUN_00010780();
    if (iVar3 != 1){
        apbStack204[8] = "MD5_Init";
        ppcVar5 = apbStack204[8];
        MD5_Init();
        apbStack204[8] = (char *)0x1;
        exit();
    }
    *(int *)((undefined *)ppcVar5 + -0x10) = plain_ptr;
    *(undefined4 *)((undefined *)ppcVar5 + -0x14) = 0x10ac4;
    len = strlen();
    *(ppcVar5-8) = len;
    *(ppcVar5-0xc) = plain_ptr;
    *(ppcVar5-0x10) = local_7c;
    *(ppcVar5-0x14) = 0x10ad7;
    puVar6 = (undefined *)ppcVar5;
    if (MD5_Update() != 1) {
      *(undefined4 *)((undefined *)ppcVar5 + -0x10) = 0x11029;
      *(undefined4 *)((undefined *)ppcVar5 + -0x14) = 0x10aee;
      FUN_000107d8();
      *(undefined4 *)((undefined *)ppcVar5 + -0x10) = 1;
      *(undefined4 *)((undefined *)ppcVar5 + -0x14) = 0x10afb;
      exit();
      puVar6 = (undefined *)ppcVar5 + -0x10;
    }
    *(char **)(puVar6 + -0xc) = local_7c;
    *(byte **)(puVar6 + -0x10) = local_20;
    *(undefined4 *)(puVar6 + -0x14) = 0x10b0b;
    puVar7 = puVar6;
    if (MD5_Final() != 1) {
        puVar7 = puVar6 + -0x10;
        *(undefined4 *)(puVar6-0x10) = 0x11034;
        *(undefined4 *)(puVar6-0x14) = 0x10b22;
        FUN_000107d8();
        *(undefined4 *)(puVar6-0x10) = 1;
        *(undefined4 *)(puVar6-0x14) = 0x10b2f;
        exit();
    }

    for(int i = 0; i<0x10; i++)
        sprintf(cipher_ptr[i], '%02x', local_20[i]);
}

