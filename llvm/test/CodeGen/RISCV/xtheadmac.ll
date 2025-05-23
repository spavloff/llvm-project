; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -mtriple=riscv32 -mattr=+xtheadmac -mattr=+m -verify-machineinstrs < %s \
; RUN:   | FileCheck %s -check-prefix=RV32XTHEADMAC
; RUN: llc -mtriple=riscv64 -mattr=+xtheadmac -mattr=+m -verify-machineinstrs < %s \
; RUN:   | FileCheck %s -check-prefix=RV64XTHEADMAC

define i32 @mula_i32(i32 %a, i32 %b, i32 %c) {
; RV32XTHEADMAC-LABEL: mula_i32:
; RV32XTHEADMAC:       # %bb.0:
; RV32XTHEADMAC-NEXT:    th.mula a0, a1, a2
; RV32XTHEADMAC-NEXT:    ret
;
; RV64XTHEADMAC-LABEL: mula_i32:
; RV64XTHEADMAC:       # %bb.0:
; RV64XTHEADMAC-NEXT:    th.mulaw a0, a1, a2
; RV64XTHEADMAC-NEXT:    ret
  %d = mul i32 %b, %c
  %e = add i32 %a, %d
  ret i32 %e
}

define i32 @muls_i32(i32 %a, i32 %b, i32 %c) {
; RV32XTHEADMAC-LABEL: muls_i32:
; RV32XTHEADMAC:       # %bb.0:
; RV32XTHEADMAC-NEXT:    th.muls a0, a1, a2
; RV32XTHEADMAC-NEXT:    ret
;
; RV64XTHEADMAC-LABEL: muls_i32:
; RV64XTHEADMAC:       # %bb.0:
; RV64XTHEADMAC-NEXT:    th.mulsw a0, a1, a2
; RV64XTHEADMAC-NEXT:    ret
  %d = mul i32 %b, %c
  %e = sub i32 %a, %d
  ret i32 %e
}

define i64 @mula_i64(i64 %a, i64 %b, i64 %c) {
; RV32XTHEADMAC-LABEL: mula_i64:
; RV32XTHEADMAC:       # %bb.0:
; RV32XTHEADMAC-NEXT:    mulhu a6, a2, a4
; RV32XTHEADMAC-NEXT:    th.mula a6, a2, a5
; RV32XTHEADMAC-NEXT:    mv a5, a0
; RV32XTHEADMAC-NEXT:    th.mula a5, a2, a4
; RV32XTHEADMAC-NEXT:    th.mula a6, a3, a4
; RV32XTHEADMAC-NEXT:    sltu a0, a5, a0
; RV32XTHEADMAC-NEXT:    add a0, a1, a0
; RV32XTHEADMAC-NEXT:    add a1, a0, a6
; RV32XTHEADMAC-NEXT:    mv a0, a5
; RV32XTHEADMAC-NEXT:    ret
;
; RV64XTHEADMAC-LABEL: mula_i64:
; RV64XTHEADMAC:       # %bb.0:
; RV64XTHEADMAC-NEXT:    th.mula a0, a1, a2
; RV64XTHEADMAC-NEXT:    ret
  %d = mul i64 %b, %c
  %f = add i64 %a, %d
  ret i64 %f
}

define i64 @mulaw_i64(i32 %a, i32 %b, i32 %c) {
; RV32XTHEADMAC-LABEL: mulaw_i64:
; RV32XTHEADMAC:       # %bb.0:
; RV32XTHEADMAC-NEXT:    th.mula a0, a1, a2
; RV32XTHEADMAC-NEXT:    srai a1, a0, 31
; RV32XTHEADMAC-NEXT:    ret
;
; RV64XTHEADMAC-LABEL: mulaw_i64:
; RV64XTHEADMAC:       # %bb.0:
; RV64XTHEADMAC-NEXT:    th.mulaw a0, a1, a2
; RV64XTHEADMAC-NEXT:    ret
  %d = mul i32 %b, %c
  %e = add i32 %a, %d
  %f = sext i32 %e to i64
  ret i64 %f
}

define i64 @mulah_i64(i32 %a, i16 %b, i16 %c) {
; RV32XTHEADMAC-LABEL: mulah_i64:
; RV32XTHEADMAC:       # %bb.0:
; RV32XTHEADMAC-NEXT:    th.mulah a0, a1, a2
; RV32XTHEADMAC-NEXT:    srai a1, a0, 31
; RV32XTHEADMAC-NEXT:    ret
;
; RV64XTHEADMAC-LABEL: mulah_i64:
; RV64XTHEADMAC:       # %bb.0:
; RV64XTHEADMAC-NEXT:    th.mulah a0, a1, a2
; RV64XTHEADMAC-NEXT:    ret
  %d = sext i16 %b to i32
  %e = sext i16 %c to i32
  %f = mul i32 %d, %e
  %g = add i32 %a, %f
  %h = sext i32 %g to i64
  ret i64 %h
}

define i64 @muls_i64(i64 %a, i64 %b, i64 %c) {
; RV32XTHEADMAC-LABEL: muls_i64:
; RV32XTHEADMAC:       # %bb.0:
; RV32XTHEADMAC-NEXT:    mulhu a6, a2, a4
; RV32XTHEADMAC-NEXT:    th.mula a6, a2, a5
; RV32XTHEADMAC-NEXT:    mul a5, a2, a4
; RV32XTHEADMAC-NEXT:    sltu a5, a0, a5
; RV32XTHEADMAC-NEXT:    th.muls a0, a2, a4
; RV32XTHEADMAC-NEXT:    th.mula a6, a3, a4
; RV32XTHEADMAC-NEXT:    sub a1, a1, a5
; RV32XTHEADMAC-NEXT:    sub a1, a1, a6
; RV32XTHEADMAC-NEXT:    ret
;
; RV64XTHEADMAC-LABEL: muls_i64:
; RV64XTHEADMAC:       # %bb.0:
; RV64XTHEADMAC-NEXT:    th.muls a0, a1, a2
; RV64XTHEADMAC-NEXT:    ret
  %d = mul i64 %b, %c
  %f = sub i64 %a, %d
  ret i64 %f
}

define i64 @mulsw_i64(i32 %a, i32 %b, i32 %c) {
; RV32XTHEADMAC-LABEL: mulsw_i64:
; RV32XTHEADMAC:       # %bb.0:
; RV32XTHEADMAC-NEXT:    th.muls a0, a1, a2
; RV32XTHEADMAC-NEXT:    srai a1, a0, 31
; RV32XTHEADMAC-NEXT:    ret
;
; RV64XTHEADMAC-LABEL: mulsw_i64:
; RV64XTHEADMAC:       # %bb.0:
; RV64XTHEADMAC-NEXT:    th.mulsw a0, a1, a2
; RV64XTHEADMAC-NEXT:    ret
  %d = mul i32 %b, %c
  %e = sub i32 %a, %d
  %f = sext i32 %e to i64
  ret i64 %f
}

define i64 @mulsh_i64(i32 %a, i16 %b, i16 %c) {
; RV32XTHEADMAC-LABEL: mulsh_i64:
; RV32XTHEADMAC:       # %bb.0:
; RV32XTHEADMAC-NEXT:    th.mulsh a0, a1, a2
; RV32XTHEADMAC-NEXT:    srai a1, a0, 31
; RV32XTHEADMAC-NEXT:    ret
;
; RV64XTHEADMAC-LABEL: mulsh_i64:
; RV64XTHEADMAC:       # %bb.0:
; RV64XTHEADMAC-NEXT:    th.mulsh a0, a1, a2
; RV64XTHEADMAC-NEXT:    ret
  %d = sext i16 %b to i32
  %e = sext i16 %c to i32
  %f = mul i32 %d, %e
  %g = sub i32 %a, %f
  %h = sext i32 %g to i64
  ret i64 %h
}

define i32 @commutative1(i32 %A, i32 %B, i32 %C) {
; RV32XTHEADMAC-LABEL: commutative1:
; RV32XTHEADMAC:       # %bb.0:
; RV32XTHEADMAC-NEXT:    th.mula a2, a1, a0
; RV32XTHEADMAC-NEXT:    mv a0, a2
; RV32XTHEADMAC-NEXT:    ret
;
; RV64XTHEADMAC-LABEL: commutative1:
; RV64XTHEADMAC:       # %bb.0:
; RV64XTHEADMAC-NEXT:    th.mulaw a2, a1, a0
; RV64XTHEADMAC-NEXT:    mv a0, a2
; RV64XTHEADMAC-NEXT:    ret
  %mul = mul nsw i32 %B, %A
  %add = add i32 %mul, %C
  ret i32 %add
}

define i32 @commutative2(i32 %A, i32 %B, i32 %C) {
; RV32XTHEADMAC-LABEL: commutative2:
; RV32XTHEADMAC:       # %bb.0:
; RV32XTHEADMAC-NEXT:    th.mula a0, a1, a2
; RV32XTHEADMAC-NEXT:    ret
;
; RV64XTHEADMAC-LABEL: commutative2:
; RV64XTHEADMAC:       # %bb.0:
; RV64XTHEADMAC-NEXT:    th.mulaw a0, a1, a2
; RV64XTHEADMAC-NEXT:    ret
  %mul = mul nsw i32 %B, %C
  %add = add i32 %mul, %A
  ret i32 %add
}

define i32 @commutative3(i32 %A, i32 %B, i32 %C) {
; RV32XTHEADMAC-LABEL: commutative3:
; RV32XTHEADMAC:       # %bb.0:
; RV32XTHEADMAC-NEXT:    th.mula a1, a2, a0
; RV32XTHEADMAC-NEXT:    mv a0, a1
; RV32XTHEADMAC-NEXT:    ret
;
; RV64XTHEADMAC-LABEL: commutative3:
; RV64XTHEADMAC:       # %bb.0:
; RV64XTHEADMAC-NEXT:    th.mulaw a1, a2, a0
; RV64XTHEADMAC-NEXT:    mv a0, a1
; RV64XTHEADMAC-NEXT:    ret
  %mul = mul nsw i32 %C, %A
  %add = add i32 %mul, %B
  ret i32 %add
}
