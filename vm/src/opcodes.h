//
//  NanoVM, a tiny java VM for the Atmel AVR family
//  Copyright (C) 2005 by Till Harbaum <Till@Harbaum.org>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// 

//
//  opcodes.h
//

#ifndef OPCODES_H
#define OPCODES_H

#define OP_NOP           0x00
#define OP_ACONST_NULL   0x01
#define OP_ICONST_M1     0x02
#define OP_ICONST_0      0x03
#define OP_ICONST_1      0x04
#define OP_ICONST_2      0x05
#define OP_ICONST_3      0x06
#define OP_ICONST_4      0x07
#define OP_ICONST_5      0x08

#define OP_FCONST_0      0x0b // only if floating point compiled in
#define OP_FCONST_1      0x0c // only if floating point compiled in
#define OP_FCONST_2      0x0d // only if floating point compiled in

#define OP_BIPUSH        0x10
#define OP_SIPUSH        0x11
#define OP_LDC           0x12


#define OP_ILOAD         0x15
#define OP_FLOAD         0x17 // only if floating point compiled in

#define OP_ILOAD_0       0x1a
#define OP_ILOAD_1       0x1b
#define OP_ILOAD_2       0x1c
#define OP_ILOAD_3       0x1d

#define OP_FLOAD_0       0x22 // only if floating point compiled in
#define OP_FLOAD_1       0x23 // only if floating point compiled in
#define OP_FLOAD_2       0x24 // only if floating point compiled in
#define OP_FLOAD_3       0x25 // only if floating point compiled in

#define OP_ALOAD_0       0x2a
#define OP_ALOAD_1       0x2b
#define OP_ALOAD_2       0x2c
#define OP_ALOAD_3       0x2d

#define OP_IALOAD        0x2e  // only if array compiled in
#define OP_FALOAD        0x30  // only if array and floating point compiled in
#define OP_AALOAD        0x32  // only if array compiled in
#define OP_BALOAD        0x33  // only if array compiled in

#define OP_ISTORE        0x36
#define OP_FSTORE        0x38 // only if floating point compiled in
#define OP_ASTORE        0x3a
#define OP_ISTORE_0      0x3b
#define OP_ISTORE_1      0x3c
#define OP_ISTORE_2      0x3d
#define OP_ISTORE_3      0x3e
#define OP_FSTORE_0      0x43 // only if floating point compiled in
#define OP_FSTORE_1      0x44 // only if floating point compiled in
#define OP_FSTORE_2      0x45 // only if floating point compiled in
#define OP_FSTORE_3      0x46 // only if floating point compiled in
#define OP_ASTORE_0      0x4b
#define OP_ASTORE_1      0x4c
#define OP_ASTORE_2      0x4d
#define OP_ASTORE_3      0x4e

#define OP_IASTORE       0x4f  // only if array compiled in
#define OP_FASTORE       0x51  // only if array and floating point compiled in
#define OP_AASTORE       0x53  // only if array compiled in
#define OP_BASTORE       0x54  // only if array compiled in

#define OP_POP           0x57
#define OP_POP2          0x58
#define OP_DUP           0x59
#define OP_DUP_X1        0x5a  // only if extedend stack ops are compiled in
#define OP_DUP_X2        0x5b  // only if extedend stack ops are compiled in
#define OP_DUP2          0x5c
#define OP_DUP2_X1       0x5d  // only if extedend stack ops are compiled in
#define OP_DUP2_X2       0x5e  // only if extedend stack ops are compiled in
#define OP_SWAP          0x5f  // only if extedend stack ops are compiled in

#define OP_IADD          0x60
#define OP_FADD          0x62 // only if floating point compiled in
#define OP_ISUB          0x64
#define OP_FSUB          0x66 // only if floating point compiled in
#define OP_IMUL          0x68
#define OP_FMUL          0x6a // only if floating point compiled in
#define OP_IDIV          0x6c
#define OP_FDIV          0x6e // only if floating point compiled in
#define OP_IREM          0x70
#define OP_FREM          0x72 // only if floating point compiled in
#define OP_INEG          0x74
#define OP_FNEG          0x76 // only if floating point compiled in
#define OP_ISHL          0x78
#define OP_ISHR          0x7a
#define OP_IUSHR         0x7c
#define OP_IAND          0x7e
#define OP_IOR           0x80
#define OP_IXOR          0x82
#define OP_IINC          0x84

#define OP_I2F           0x86 // only if floating point compiled in
#define OP_F2I           0x8b // only if floating point compiled in

#define OP_FCMPL         0x95 // only if floating point compiled in
#define OP_FCMPG         0x96 // only if floating point compiled in

#define OP_IFEQ          0x99
#define OP_IFNE          0x9a
#define OP_IFLT          0x9b
#define OP_IFGE          0x9c
#define OP_IFGT          0x9d
#define OP_IFLE          0x9e

#define OP_IF_ICMPEQ     0x9f
#define OP_IF_ICMPNE     0xa0
#define OP_IF_ICMPLT     0xa1
#define OP_IF_ICMPGE     0xa2
#define OP_IF_ICMPGT     0xa3
#define OP_IF_ICMPLE     0xa4

#define OP_GOTO          0xa7

#define OP_TABLESWITCH   0xaa
#define OP_LOOKUPSWITCH  0xab

#define OP_IRETURN       0xac
#define OP_FRETURN       0xae // only if floating point compiled in
#define OP_RETURN        0xb1

#define OP_GETSTATIC     0xb2
#define OP_PUTSTATIC     0xb3  // only if fields are compiled in
#define OP_GETFIELD      0xb4  // only if fields are compiled in
#define OP_PUTFIELD      0xb5  // only if fields are compiled in
#define OP_INVOKEVIRTUAL 0xb6
#define OP_INVOKESPECIAL 0xb7
#define OP_INVOKESTATIC  0xb8

#define OP_NEW           0xbb
#define OP_NEWARRAY      0xbc  // only if array compiled in
#define OP_ANEWARRAY     0xbd  // only if array compiled in
#define OP_ARRAYLENGTH   0xbe  // only if array compiled in

#endif // OPCODES_H
