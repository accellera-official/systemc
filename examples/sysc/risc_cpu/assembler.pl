#!/usr/local/bin/perl
#/******************************************************************************
#  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
#  more contributor license agreements.  See the NOTICE file distributed
#  with this work for additional information regarding copyright ownership.
#  Accellera licenses this file to you under the Apache License, Version 2.0
#  (the "License"); you may not use this file except in compliance with the
#  License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
#  implied.  See the License for the specific language governing
#  permissions and limitations under the License.
#
#******************************************************************************/
#/******************************************************************************
# 
#    assembler.pl -- Simple Assembler in Perl
# 
#    Original Author: Martin Wang. Synopsys, Inc. (mwang@synopsys.com)
# 
#******************************************************************************/
# 
#/******************************************************************************
# 
#    MODIFICATION LOG - modifiers, enter your name, affliation and
#    changes you are making here:
# 
#    Modifier Name & Affiliation:
#    Description of Modification:
# 
# 
#******************************************************************************/
# 
#/***************************************************************************
# 
#   Copyright (c) 1998-1999 Synopsys, Inc.
#   ALL RIGHTS RESERVED
# 
#*****************************************************************************/
#
#Change the above line for your perl

if ($ARGV[1] eq "-code") {
	$CODEGEN = 1;
}else {
	$CODEGEN = 0;
}
#print "Input source name = $ARGV[0]\n";

$pc = 5;
$filename = $ARGV[0];
open(FP,"$filename"); 

printf ("0x00000000\n");
printf ("0x00000000\n");
printf ("0x00000000\n");
printf ("0x00000000\n");
printf ("0x00000000\n");
while(<FP>){
	chop($_);
	$_ =~ s/\s+/ /g;
	($opcode, $destC, $sourceA, $sourceB) = split(/ /,$_);
	$destC =~ s/R//g;
	$sourceA =~ s/R//g;
	$sourceB =~ s/R//g;
	if ($opcode =~ /\badd\b/) {
		printf ("0x01");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bmod\b/) {
		printf ("0x0e");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\baddi\b/) {
		printf ("0x02");
		printf ("%01x%01x%04x",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\baddc\b/) {
		printf ("0x03");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bsub\b/) {
		printf ("0x04");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bsubi\b/) {
		printf ("0x05");
		printf ("%01x%01x%04x",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bsubc\b/) {
		printf ("0x06");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bmul\b/) {
		printf ("0x07");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bdiv\b/) {
		printf ("0x08");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bnand\b/) {
		printf ("0x09");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\band\b/) {
		printf ("0x0a");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bor\b/) {
		printf ("0x0b");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bxor\b/) {
		printf ("0x0c");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bnot\b/) {
		printf ("0x0d");
		printf ("%01x%01x0000",$destC, $sourceA);
	}
	if ($opcode =~ /\blw\b/) {
		printf ("0x4d");
		printf ("%01x%01x%04x",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bsw\b/) {
		printf ("0x4e");
		printf ("%01x%01x%04x",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bmov\b/) {
		printf ("0x0f");
		printf ("%01x%01x%01x000",$destC, $sourceA, 0);
	}
	if ($opcode =~ /\bmovi\b/) {
		printf ("0xf1");
		printf ("%01x%05x",$destC, $sourceA);
	}
	if ($opcode =~ /\bbeq\b/) {
		printf ("0x10");
                $sourceB = 0x0000ffff & $sourceB;
		printf ("%01x%01x%04x",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bbne\b/) {
		printf ("0x11");
                $sourceB = 0x0000ffff & $sourceB;
		printf ("%01x%01x%04x",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bbgt\b/) {
		printf ("0x12");
                $sourceB = 0x0000ffff & $sourceB;
		printf ("%01x%01x%04x",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bbge\b/) {
		printf ("0x13");
                $sourceB = 0x0000ffff & $sourceB;
		printf ("%01x%01x%04x",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bblt\b/) {
		printf ("0x14");
                $sourceB = 0x0000ffff & $sourceB;
		printf ("%01x%01x%04x",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bble\b/) {
		printf ("0x15");
                $sourceB = 0x0000ffff & $sourceB;
		printf ("%01x%01x%04x",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bj\b/) {
		printf ("0x16");
                $sourceB = 0x0000ffff & $sourceB;
		printf ("%06x",$destC);
	}
	if ($opcode =~ /\bfadd\b/) {
		printf ("0x29");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bfsub\b/) {
		printf ("0x2a");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bfmul\b/) {
		printf ("0x2b");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bfdiv\b/) {
		printf ("0x2c");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bmmxadd\b/) {
		printf ("0x31");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bmmxadds\b/) {
		printf ("0x32");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bmmxsub\b/) {
		printf ("0x33");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bmmxsubs\b/) {
		printf ("0x34");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bpmadd\b/) {
		printf ("0x35");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bpack\b/) {
		printf ("0x36");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bmmxck\b/) {
		printf ("0x37");
		printf ("%01x%01x%01x000",$destC, $sourceA, $sourceB);
	}
	if ($opcode =~ /\bldpid\b/) {
		printf ("0xf0");
		printf ("%06x",$destC);
	}
	if ($opcode =~ /\bflush\b/) {
		printf ("0xe0000000");
	}
	if ($CODEGEN) {
		printf ("\n");
	} else {
		printf("\t/* %s * pc = 0x%x=%d/\n",$_, $pc, $pc);
	}
	$pc ++;
	if ($CODEGEN) {
		printf ("0x00000000\n");
	} else {
		printf ("0x00000000");
		printf("\t\t\t\t\t/* REGISTER DUMP * pc = %d/\n", $pc);
	}
	$pc ++;
			


}
printf ("0xffffffff\n");
