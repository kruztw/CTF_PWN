#!/bin/bash

rule="`seccomp-tools asm rules.asm`"
echo $rule

