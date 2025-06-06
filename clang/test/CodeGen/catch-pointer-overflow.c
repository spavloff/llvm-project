// RUN: %clang_cc1 -x c -emit-llvm %s -o - -triple x86_64-linux-gnu | FileCheck %s
// RUN: %clang_cc1 -x c -fsanitize=pointer-overflow -fno-sanitize-recover=pointer-overflow -emit-llvm %s -o - -triple x86_64-linux-gnu | FileCheck %s -implicit-check-not="call void @__ubsan_handle_pointer_overflow" --check-prefixes=CHECK,CHECK-SANITIZE,CHECK-SANITIZE-ANYRECOVER,CHECK-SANITIZE-NORECOVER,CHECK-SANITIZE-UNREACHABLE
// RUN: %clang_cc1 -x c -fsanitize=pointer-overflow -fsanitize-recover=pointer-overflow -emit-llvm %s -o - -triple x86_64-linux-gnu | FileCheck %s -implicit-check-not="call void @__ubsan_handle_pointer_overflow" --check-prefixes=CHECK,CHECK-SANITIZE,CHECK-SANITIZE-ANYRECOVER,CHECK-SANITIZE-RECOVER
// RUN: %clang_cc1 -x c -fsanitize=pointer-overflow -fsanitize-trap=pointer-overflow -emit-llvm %s -o - -triple x86_64-linux-gnu | FileCheck %s -implicit-check-not="call void @__ubsan_handle_pointer_overflow" --check-prefixes=CHECK,CHECK-SANITIZE,CHECK-SANITIZE-TRAP,CHECK-SANITIZE-UNREACHABLE

// RUN: %clang_cc1 -x c++ -emit-llvm %s -o - -triple x86_64-linux-gnu | FileCheck %s
// RUN: %clang_cc1 -x c++ -fsanitize=pointer-overflow -fno-sanitize-recover=pointer-overflow -emit-llvm %s -o - -triple x86_64-linux-gnu | FileCheck %s -implicit-check-not="call void @__ubsan_handle_pointer_overflow" --check-prefixes=CHECK,CHECK-SANITIZE,CHECK-SANITIZE-ANYRECOVER,CHECK-SANITIZE-NORECOVER,CHECK-SANITIZE-UNREACHABLE
// RUN: %clang_cc1 -x c++ -fsanitize=pointer-overflow -fsanitize-recover=pointer-overflow -emit-llvm %s -o - -triple x86_64-linux-gnu | FileCheck %s -implicit-check-not="call void @__ubsan_handle_pointer_overflow" --check-prefixes=CHECK,CHECK-SANITIZE,CHECK-SANITIZE-ANYRECOVER,CHECK-SANITIZE-RECOVER
// RUN: %clang_cc1 -x c++ -fsanitize=pointer-overflow -fsanitize-trap=pointer-overflow -emit-llvm %s -o - -triple x86_64-linux-gnu | FileCheck %s -implicit-check-not="call void @__ubsan_handle_pointer_overflow" --check-prefixes=CHECK,CHECK-SANITIZE,CHECK-SANITIZE-TRAP,CHECK-SANITIZE-UNREACHABLE

// CHECK-SANITIZE-ANYRECOVER-DAG: @[[LINE_100:.*]] = {{.*}}, i32 100, i32 15 } }
// CHECK-SANITIZE-ANYRECOVER-DAG: @[[LINE_200:.*]] = {{.*}}, i32 200, i32 15 } }
// CHECK-SANITIZE-ANYRECOVER-DAG: @[[LINE_300:.*]] = {{.*}}, i32 300, i32 15 } }
// CHECK-SANITIZE-ANYRECOVER-DAG: @[[LINE_400:.*]] = {{.*}}, i32 400, i32 15 } }
// CHECK-SANITIZE-ANYRECOVER-DAG: @[[LINE_500:.*]] = {{.*}}, i32 500, i32 7 } }
// CHECK-SANITIZE-ANYRECOVER-DAG: @[[LINE_600:.*]] = {{.*}}, i32 600, i32 7 } }
// CHECK-SANITIZE-ANYRECOVER-DAG: @[[LINE_700:.*]] = {{.*}}, i32 700, i32 3 } }
// CHECK-SANITIZE-ANYRECOVER-DAG: @[[LINE_800:.*]] = {{.*}}, i32 800, i32 3 } }

#ifdef __cplusplus
extern "C" {
#endif

char *add_unsigned(char *base, unsigned long offset) {
  // CHECK:                           define{{.*}} ptr @add_unsigned(ptr noundef %[[BASE:.*]], i64 noundef %[[OFFSET:.*]])
  // CHECK-NEXT:                      [[ENTRY:.*]]:
  // CHECK-NEXT:                        %[[BASE_ADDR:.*]] = alloca ptr, align 8
  // CHECK-NEXT:                        %[[OFFSET_ADDR:.*]] = alloca i64, align 8
  // CHECK-NEXT:                        store ptr %[[BASE]], ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        store i64 %[[OFFSET]], ptr %[[OFFSET_ADDR]], align 8
  // CHECK-NEXT:                        %[[BASE_RELOADED:.*]] = load ptr, ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        %[[OFFSET_RELOADED:.*]] = load i64, ptr %[[OFFSET_ADDR]], align 8
  // CHECK-NEXT:                        %[[ADD_PTR:.*]] = getelementptr inbounds nuw i8, ptr %[[BASE_RELOADED]], i64 %[[OFFSET_RELOADED]]
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_OFFSET_AGGREGATE:.*]] = call { i64, i1 } @llvm.smul.with.overflow.i64(i64 1, i64 %[[OFFSET_RELOADED]]), !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_OFFSET_OVERFLOWED:.*]] = extractvalue { i64, i1 } %[[COMPUTED_OFFSET_AGGREGATE]], 1, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[OR_OV:.+]] = or i1 %[[COMPUTED_OFFSET_OVERFLOWED]], false, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_OFFSET:.*]] = extractvalue { i64, i1 } %[[COMPUTED_OFFSET_AGGREGATE]], 0, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BASE_RELOADED_INT:.*]] = ptrtoint ptr %[[BASE_RELOADED]] to i64, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP:.*]] = add i64 %[[BASE_RELOADED_INT]], %[[COMPUTED_OFFSET]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BASE_IS_NOT_NULLPTR:.*]] = icmp ne ptr %[[BASE_RELOADED]], null, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP_IS_NOT_NULL:.*]] = icmp ne i64 %[[COMPUTED_GEP]], 0, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BOTH_POINTERS_ARE_NULL_OR_BOTH_ARE_NONNULL:.*]] = icmp eq i1 %[[BASE_IS_NOT_NULLPTR]], %[[COMPUTED_GEP_IS_NOT_NULL]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_OFFSET_DID_NOT_OVERFLOW:.*]] = xor i1 %[[OR_OV]], true, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP_IS_UGE_BASE:.*]] = icmp uge i64 %[[COMPUTED_GEP]], %[[BASE_RELOADED_INT]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[GEP_DID_NOT_OVERFLOW:.*]] = and i1 %[[COMPUTED_GEP_IS_UGE_BASE]], %[[COMPUTED_OFFSET_DID_NOT_OVERFLOW]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[GEP_IS_OKAY:.*]] = and i1 %[[BOTH_POINTERS_ARE_NULL_OR_BOTH_ARE_NONNULL]], %[[GEP_DID_NOT_OVERFLOW]], !nosanitize
  // CHECK-SANITIZE-NEXT:               br i1 %[[GEP_IS_OKAY]], label %[[CONT:.*]], label %[[HANDLER_POINTER_OVERFLOW:[^,]+]],{{.*}} !nosanitize
  // CHECK-SANITIZE:                  [[HANDLER_POINTER_OVERFLOW]]:
  // CHECK-SANITIZE-NORECOVER-NEXT:     call void @__ubsan_handle_pointer_overflow_abort(ptr @[[LINE_100]], i64 %[[BASE_RELOADED_INT]], i64 %[[COMPUTED_GEP]])
  // CHECK-SANITIZE-RECOVER-NEXT:       call void @__ubsan_handle_pointer_overflow(ptr @[[LINE_100]], i64 %[[BASE_RELOADED_INT]], i64 %[[COMPUTED_GEP]])
  // CHECK-SANITIZE-TRAP-NEXT:          call void @llvm.ubsantrap(i8 19){{.*}}, !nosanitize
  // CHECK-SANITIZE-UNREACHABLE-NEXT:   unreachable, !nosanitize
  // CHECK-SANITIZE:                  [[CONT]]:
  // CHECK-NEXT:                        ret ptr %[[ADD_PTR]]
#line 100
  return base + offset;
}

char *sub_unsigned(char *base, unsigned long offset) {
  // CHECK:                           define{{.*}} ptr @sub_unsigned(ptr noundef %[[BASE:.*]], i64 noundef %[[OFFSET:.*]])
  // CHECK-NEXT:                      [[ENTRY:.*]]:
  // CHECK-NEXT:                        %[[BASE_ADDR:.*]] = alloca ptr, align 8
  // CHECK-NEXT:                        %[[OFFSET_ADDR:.*]] = alloca i64, align 8
  // CHECK-NEXT:                        store ptr %[[BASE]], ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        store i64 %[[OFFSET]], ptr %[[OFFSET_ADDR]], align 8
  // CHECK-NEXT:                        %[[BASE_RELOADED:.*]] = load ptr, ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        %[[OFFSET_RELOADED:.*]] = load i64, ptr %[[OFFSET_ADDR]], align 8
  // CHECK-NEXT:                        %[[IDX_NEG:.*]] = sub i64 0, %[[OFFSET_RELOADED]]
  // CHECK-NEXT:                        %[[ADD_PTR:.*]] = getelementptr inbounds i8, ptr %[[BASE_RELOADED]], i64 %[[IDX_NEG]]
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_OFFSET_AGGREGATE:.*]] = call { i64, i1 } @llvm.smul.with.overflow.i64(i64 1, i64 %[[IDX_NEG]]), !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_OFFSET_OVERFLOWED:.*]] = extractvalue { i64, i1 } %[[COMPUTED_OFFSET_AGGREGATE]], 1, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[OR_OV:.+]] = or i1 %[[COMPUTED_OFFSET_OVERFLOWED]], false, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_OFFSET:.*]] = extractvalue { i64, i1 } %[[COMPUTED_OFFSET_AGGREGATE]], 0, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BASE_RELOADED_INT:.*]] = ptrtoint ptr %[[BASE_RELOADED]] to i64, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP:.*]] = add i64 %[[BASE_RELOADED_INT]], %[[COMPUTED_OFFSET]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BASE_IS_NOT_NULLPTR:.*]] = icmp ne ptr %[[BASE_RELOADED]], null, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP_IS_NOT_NULL:.*]] = icmp ne i64 %[[COMPUTED_GEP]], 0, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BOTH_POINTERS_ARE_NULL_OR_BOTH_ARE_NONNULL:.*]] = icmp eq i1 %[[BASE_IS_NOT_NULLPTR]], %[[COMPUTED_GEP_IS_NOT_NULL]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_OFFSET_DID_NOT_OVERFLOW:.*]] = xor i1 %[[OR_OV]], true, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP_IS_ULE_BASE:.*]] = icmp ule i64 %[[COMPUTED_GEP]], %[[BASE_RELOADED_INT]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[GEP_DID_NOT_OVERFLOW:.*]] = and i1 %[[COMPUTED_GEP_IS_ULE_BASE]], %[[COMPUTED_OFFSET_DID_NOT_OVERFLOW]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[GEP_IS_OKAY:.*]] = and i1 %[[BOTH_POINTERS_ARE_NULL_OR_BOTH_ARE_NONNULL]], %[[GEP_DID_NOT_OVERFLOW]], !nosanitize
  // CHECK-SANITIZE-NEXT:               br i1 %[[GEP_IS_OKAY]], label %[[CONT:.*]], label %[[HANDLER_POINTER_OVERFLOW:[^,]+]],{{.*}} !nosanitize
  // CHECK-SANITIZE:                  [[HANDLER_POINTER_OVERFLOW]]:
  // CHECK-SANITIZE-NORECOVER-NEXT:     call void @__ubsan_handle_pointer_overflow_abort(ptr @[[LINE_200]], i64 %[[BASE_RELOADED_INT]], i64 %[[COMPUTED_GEP]])
  // CHECK-SANITIZE-RECOVER-NEXT:       call void @__ubsan_handle_pointer_overflow(ptr @[[LINE_200]], i64 %[[BASE_RELOADED_INT]], i64 %[[COMPUTED_GEP]])
  // CHECK-SANITIZE-TRAP-NEXT:          call void @llvm.ubsantrap(i8 19){{.*}}, !nosanitize
  // CHECK-SANITIZE-UNREACHABLE-NEXT:   unreachable, !nosanitize
  // CHECK-SANITIZE:                  [[CONT]]:
  // CHECK-NEXT:                        ret ptr %[[ADD_PTR]]
#line 200
  return base - offset;
}

char *add_signed(char *base, signed long offset) {
  // CHECK:                           define{{.*}} ptr @add_signed(ptr noundef %[[BASE:.*]], i64 noundef %[[OFFSET:.*]])
  // CHECK-NEXT:                      [[ENTRY:.*]]:
  // CHECK-NEXT:                        %[[BASE_ADDR:.*]] = alloca ptr, align 8
  // CHECK-NEXT:                        %[[OFFSET_ADDR:.*]] = alloca i64, align 8
  // CHECK-NEXT:                        store ptr %[[BASE]], ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        store i64 %[[OFFSET]], ptr %[[OFFSET_ADDR]], align 8
  // CHECK-NEXT:                        %[[BASE_RELOADED:.*]] = load ptr, ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        %[[OFFSET_RELOADED:.*]] = load i64, ptr %[[OFFSET_ADDR]], align 8
  // CHECK-NEXT:                        %[[ADD_PTR:.*]] = getelementptr inbounds i8, ptr %[[BASE_RELOADED]], i64 %[[OFFSET_RELOADED]]
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_OFFSET_AGGREGATE:.*]] = call { i64, i1 } @llvm.smul.with.overflow.i64(i64 1, i64 %[[OFFSET_RELOADED]]), !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_OFFSET_OVERFLOWED:.*]] = extractvalue { i64, i1 } %[[COMPUTED_OFFSET_AGGREGATE]], 1, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[OR_OV:.+]] = or i1 %[[COMPUTED_OFFSET_OVERFLOWED]], false, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_OFFSET:.*]] = extractvalue { i64, i1 } %[[COMPUTED_OFFSET_AGGREGATE]], 0, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BASE_RELOADED_INT:.*]] = ptrtoint ptr %[[BASE_RELOADED]] to i64, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP:.*]] = add i64 %[[BASE_RELOADED_INT]], %[[COMPUTED_OFFSET]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BASE_IS_NOT_NULLPTR:.*]] = icmp ne ptr %[[BASE_RELOADED]], null, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP_IS_NOT_NULL:.*]] = icmp ne i64 %[[COMPUTED_GEP]], 0, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BOTH_POINTERS_ARE_NULL_OR_BOTH_ARE_NONNULL:.*]] = icmp eq i1 %[[BASE_IS_NOT_NULLPTR]], %[[COMPUTED_GEP_IS_NOT_NULL]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_OFFSET_DID_NOT_OVERFLOW:.*]] = xor i1 %[[OR_OV]], true, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[POSORZEROVALID:.*]] = icmp uge i64 %[[COMPUTED_GEP]], %[[BASE_RELOADED_INT]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[POSORZEROOFFSET:.*]] = icmp sge i64 %[[COMPUTED_OFFSET]], 0, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[NEGVALID:.*]] = icmp ult i64 %[[COMPUTED_GEP]], %[[BASE_RELOADED_INT]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[APPLYING_OFFSET_DID_NOT_OVERFLOW:.*]] = select i1 %[[POSORZEROOFFSET]], i1 %[[POSORZEROVALID]], i1 %[[NEGVALID]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[GEP_DID_NOT_OVERFLOW:.*]] = and i1 %[[APPLYING_OFFSET_DID_NOT_OVERFLOW]], %[[COMPUTED_OFFSET_DID_NOT_OVERFLOW]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[GEP_IS_OKAY:.*]] = and i1 %[[BOTH_POINTERS_ARE_NULL_OR_BOTH_ARE_NONNULL]], %[[GEP_DID_NOT_OVERFLOW]], !nosanitize
  // CHECK-SANITIZE-NEXT:               br i1 %[[GEP_IS_OKAY]], label %[[CONT:.*]], label %[[HANDLER_POINTER_OVERFLOW:[^,]+]],{{.*}} !nosanitize
  // CHECK-SANITIZE:                  [[HANDLER_POINTER_OVERFLOW]]:
  // CHECK-SANITIZE-NORECOVER-NEXT:     call void @__ubsan_handle_pointer_overflow_abort(ptr @[[LINE_300]], i64 %[[BASE_RELOADED_INT]], i64 %[[COMPUTED_GEP]])
  // CHECK-SANITIZE-RECOVER-NEXT:       call void @__ubsan_handle_pointer_overflow(ptr @[[LINE_300]], i64 %[[BASE_RELOADED_INT]], i64 %[[COMPUTED_GEP]])
  // CHECK-SANITIZE-TRAP-NEXT:          call void @llvm.ubsantrap(i8 19){{.*}}, !nosanitize
  // CHECK-SANITIZE-UNREACHABLE-NEXT:   unreachable, !nosanitize
  // CHECK-SANITIZE:                  [[CONT]]:
  // CHECK-NEXT:                        ret ptr %[[ADD_PTR]]
#line 300
  return base + offset;
}

char *sub_signed(char *base, signed long offset) {
  // CHECK:                           define{{.*}} ptr @sub_signed(ptr noundef %[[BASE:.*]], i64 noundef %[[OFFSET:.*]])
  // CHECK-NEXT:                      [[ENTRY:.*]]:
  // CHECK-NEXT:                        %[[BASE_ADDR:.*]] = alloca ptr, align 8
  // CHECK-NEXT:                        %[[OFFSET_ADDR:.*]] = alloca i64, align 8
  // CHECK-NEXT:                        store ptr %[[BASE]], ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        store i64 %[[OFFSET]], ptr %[[OFFSET_ADDR]], align 8
  // CHECK-NEXT:                        %[[BASE_RELOADED:.*]] = load ptr, ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        %[[OFFSET_RELOADED:.*]] = load i64, ptr %[[OFFSET_ADDR]], align 8
  // CHECK-NEXT:                        %[[IDX_NEG:.*]] = sub i64 0, %[[OFFSET_RELOADED]]
  // CHECK-NEXT:                        %[[ADD_PTR:.*]] = getelementptr inbounds i8, ptr %[[BASE_RELOADED]], i64 %[[IDX_NEG]]
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_OFFSET_AGGREGATE:.*]] = call { i64, i1 } @llvm.smul.with.overflow.i64(i64 1, i64 %[[IDX_NEG]]), !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_OFFSET_OVERFLOWED:.*]] = extractvalue { i64, i1 } %[[COMPUTED_OFFSET_AGGREGATE]], 1, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[OR_OV:.+]] = or i1 %[[COMPUTED_OFFSET_OVERFLOWED]], false, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_OFFSET:.*]] = extractvalue { i64, i1 } %[[COMPUTED_OFFSET_AGGREGATE]], 0, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BASE_RELOADED_INT:.*]] = ptrtoint ptr %[[BASE_RELOADED]] to i64, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP:.*]] = add i64 %[[BASE_RELOADED_INT]], %[[COMPUTED_OFFSET]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BASE_IS_NOT_NULLPTR:.*]] = icmp ne ptr %[[BASE_RELOADED]], null, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP_IS_NOT_NULL:.*]] = icmp ne i64 %[[COMPUTED_GEP]], 0, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BOTH_POINTERS_ARE_NULL_OR_BOTH_ARE_NONNULL:.*]] = icmp eq i1 %[[BASE_IS_NOT_NULLPTR]], %[[COMPUTED_GEP_IS_NOT_NULL]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_OFFSET_DID_NOT_OVERFLOW:.*]] = xor i1 %[[OR_OV]], true, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[POSORZEROVALID:.*]] = icmp uge i64 %[[COMPUTED_GEP]], %[[BASE_RELOADED_INT]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[POSORZEROOFFSET:.*]] = icmp sge i64 %[[COMPUTED_OFFSET]], 0, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[NEGVALID:.*]] = icmp ult i64 %[[COMPUTED_GEP]], %[[BASE_RELOADED_INT]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[APPLYING_OFFSET_DID_NOT_OVERFLOW:.*]] = select i1 %[[POSORZEROOFFSET]], i1 %[[POSORZEROVALID]], i1 %[[NEGVALID]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[GEP_DID_NOT_OVERFLOW:.*]] = and i1 %[[APPLYING_OFFSET_DID_NOT_OVERFLOW]], %[[COMPUTED_OFFSET_DID_NOT_OVERFLOW]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[GEP_IS_OKAY:.*]] = and i1 %[[BOTH_POINTERS_ARE_NULL_OR_BOTH_ARE_NONNULL]], %[[GEP_DID_NOT_OVERFLOW]], !nosanitize
  // CHECK-SANITIZE-NEXT:               br i1 %[[GEP_IS_OKAY]], label %[[CONT:.*]], label %[[HANDLER_POINTER_OVERFLOW:[^,]+]],{{.*}} !nosanitize
  // CHECK-SANITIZE:                  [[HANDLER_POINTER_OVERFLOW]]:
  // CHECK-SANITIZE-NORECOVER-NEXT:     call void @__ubsan_handle_pointer_overflow_abort(ptr @[[LINE_400]], i64 %[[BASE_RELOADED_INT]], i64 %[[COMPUTED_GEP]])
  // CHECK-SANITIZE-RECOVER-NEXT:       call void @__ubsan_handle_pointer_overflow(ptr @[[LINE_400]], i64 %[[BASE_RELOADED_INT]], i64 %[[COMPUTED_GEP]])
  // CHECK-SANITIZE-TRAP-NEXT:          call void @llvm.ubsantrap(i8 19){{.*}}, !nosanitize
  // CHECK-SANITIZE-UNREACHABLE-NEXT:   unreachable, !nosanitize
  // CHECK-SANITIZE:                  [[CONT]]:
  // CHECK-NEXT:                        ret ptr %[[ADD_PTR]]
#line 400
  return base - offset;
}

char *postinc(char *base) {
  // CHECK:                           define{{.*}} ptr @postinc(ptr noundef %[[BASE:.*]])
  // CHECK-NEXT:                      [[ENTRY:.*]]:
  // CHECK-NEXT:                        %[[BASE_ADDR:.*]] = alloca ptr, align 8
  // CHECK-NEXT:                        store ptr %[[BASE]], ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        %[[BASE_RELOADED:.*]] = load ptr, ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        %[[ADD_PTR:.*]] = getelementptr inbounds nuw i8, ptr %[[BASE_RELOADED]], i32 1
  // CHECK-SANITIZE-NEXT:               %[[BASE_RELOADED_INT:.*]] = ptrtoint ptr %[[BASE_RELOADED]] to i64, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP:.*]] = add i64 %[[BASE_RELOADED_INT]], 1, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BASE_IS_NOT_NULLPTR:.*]] = icmp ne ptr %[[BASE_RELOADED]], null, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP_IS_NOT_NULL:.*]] = icmp ne i64 %[[COMPUTED_GEP]], 0, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BOTH_POINTERS_ARE_NULL_OR_BOTH_ARE_NONNULL:.*]] = icmp eq i1 %[[BASE_IS_NOT_NULLPTR]], %[[COMPUTED_GEP_IS_NOT_NULL]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP_IS_UGE_BASE:.*]] = icmp uge i64 %[[COMPUTED_GEP]], %[[BASE_RELOADED_INT]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[AND_TRUE:.*]] = and i1 %[[COMPUTED_GEP_IS_UGE_BASE]], true, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[GEP_IS_OKAY:.*]] = and i1 %[[BOTH_POINTERS_ARE_NULL_OR_BOTH_ARE_NONNULL]], %[[AND_TRUE]], !nosanitize
  // CHECK-SANITIZE-NEXT:               br i1 %[[GEP_IS_OKAY]], label %[[CONT:.*]], label %[[HANDLER_POINTER_OVERFLOW:[^,]+]],{{.*}} !nosanitize
  // CHECK-SANITIZE:                  [[HANDLER_POINTER_OVERFLOW]]:
  // CHECK-SANITIZE-NORECOVER-NEXT:     call void @__ubsan_handle_pointer_overflow_abort(ptr @[[LINE_500]], i64 %[[BASE_RELOADED_INT]], i64 %[[COMPUTED_GEP]])
  // CHECK-SANITIZE-RECOVER-NEXT:       call void @__ubsan_handle_pointer_overflow(ptr @[[LINE_500]], i64 %[[BASE_RELOADED_INT]], i64 %[[COMPUTED_GEP]])
  // CHECK-SANITIZE-TRAP-NEXT:          call void @llvm.ubsantrap(i8 19){{.*}}, !nosanitize
  // CHECK-SANITIZE-UNREACHABLE-NEXT:   unreachable, !nosanitize
  // CHECK-SANITIZE:                  [[CONT]]:
  // CHECK-NEXT:                        store ptr %[[ADD_PTR]], ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        %[[ADD_PTR_RELOADED:.*]] = load ptr, ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        ret ptr %[[ADD_PTR_RELOADED]]
#line 500
  base++;
  return base;
}

char *postdec(char *base) {
  // CHECK:                           define{{.*}} ptr @postdec(ptr noundef %[[BASE:.*]])
  // CHECK-NEXT:                      [[ENTRY:.*]]:
  // CHECK-NEXT:                        %[[BASE_ADDR:.*]] = alloca ptr, align 8
  // CHECK-NEXT:                        store ptr %[[BASE]], ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        %[[BASE_RELOADED:.*]] = load ptr, ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        %[[ADD_PTR:.*]] = getelementptr inbounds i8, ptr %[[BASE_RELOADED]], i32 -1
  // CHECK-SANITIZE-NEXT:               %[[BASE_RELOADED_INT:.*]] = ptrtoint ptr %[[BASE_RELOADED]] to i64, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP:.*]] = add i64 %[[BASE_RELOADED_INT]], -1, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BASE_IS_NOT_NULLPTR:.*]] = icmp ne ptr %[[BASE_RELOADED]], null, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP_IS_NOT_NULL:.*]] = icmp ne i64 %[[COMPUTED_GEP]], 0, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BOTH_POINTERS_ARE_NULL_OR_BOTH_ARE_NONNULL:.*]] = icmp eq i1 %[[BASE_IS_NOT_NULLPTR]], %[[COMPUTED_GEP_IS_NOT_NULL]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP_IS_ULE_BASE:.*]] = icmp ule i64 %[[COMPUTED_GEP]], %[[BASE_RELOADED_INT]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[AND_TRUE:.*]] = and i1 %[[COMPUTED_GEP_IS_UGE_BASE]], true, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[GEP_IS_OKAY:.*]] = and i1 %[[BOTH_POINTERS_ARE_NULL_OR_BOTH_ARE_NONNULL]], %[[AND_TRUE]], !nosanitize
  // CHECK-SANITIZE-NEXT:               br i1 %[[GEP_IS_OKAY]], label %[[CONT:.*]], label %[[HANDLER_POINTER_OVERFLOW:[^,]+]],{{.*}} !nosanitize
  // CHECK-SANITIZE:                  [[HANDLER_POINTER_OVERFLOW]]:
  // CHECK-SANITIZE-NORECOVER-NEXT:     call void @__ubsan_handle_pointer_overflow_abort(ptr @[[LINE_600]], i64 %[[BASE_RELOADED_INT]], i64 %[[COMPUTED_GEP]])
  // CHECK-SANITIZE-RECOVER-NEXT:       call void @__ubsan_handle_pointer_overflow(ptr @[[LINE_600]], i64 %[[BASE_RELOADED_INT]], i64 %[[COMPUTED_GEP]])
  // CHECK-SANITIZE-TRAP-NEXT:          call void @llvm.ubsantrap(i8 19){{.*}}, !nosanitize
  // CHECK-SANITIZE-UNREACHABLE-NEXT:   unreachable, !nosanitize
  // CHECK-SANITIZE:                  [[CONT]]:
  // CHECK-NEXT:                        store ptr %[[ADD_PTR]], ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        %[[ADD_PTR_RELOADED:.*]] = load ptr, ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        ret ptr %[[ADD_PTR_RELOADED]]
#line 600
  base--;
  return base;
}

char *preinc(char *base) {
  // CHECK:                           define{{.*}} ptr @preinc(ptr noundef %[[BASE:.*]])
  // CHECK-NEXT:                      [[ENTRY:.*]]:
  // CHECK-NEXT:                        %[[BASE_ADDR:.*]] = alloca ptr, align 8
  // CHECK-NEXT:                        store ptr %[[BASE]], ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        %[[BASE_RELOADED:.*]] = load ptr, ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        %[[ADD_PTR:.*]] = getelementptr inbounds nuw i8, ptr %[[BASE_RELOADED]], i32 1
  // CHECK-SANITIZE-NEXT:               %[[BASE_RELOADED_INT:.*]] = ptrtoint ptr %[[BASE_RELOADED]] to i64, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP:.*]] = add i64 %[[BASE_RELOADED_INT]], 1, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BASE_IS_NOT_NULLPTR:.*]] = icmp ne ptr %[[BASE_RELOADED]], null, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP_IS_NOT_NULL:.*]] = icmp ne i64 %[[COMPUTED_GEP]], 0, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BOTH_POINTERS_ARE_NULL_OR_BOTH_ARE_NONNULL:.*]] = icmp eq i1 %[[BASE_IS_NOT_NULLPTR]], %[[COMPUTED_GEP_IS_NOT_NULL]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP_IS_UGE_BASE:.*]] = icmp uge i64 %[[COMPUTED_GEP]], %[[BASE_RELOADED_INT]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[AND_TRUE:.*]] = and i1 %[[COMPUTED_GEP_IS_UGE_BASE]], true, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[GEP_IS_OKAY:.*]] = and i1 %[[BOTH_POINTERS_ARE_NULL_OR_BOTH_ARE_NONNULL]], %[[AND_TRUE]], !nosanitize
  // CHECK-SANITIZE-NEXT:               br i1 %[[GEP_IS_OKAY]], label %[[CONT:.*]], label %[[HANDLER_POINTER_OVERFLOW:[^,]+]],{{.*}} !nosanitize
  // CHECK-SANITIZE:                  [[HANDLER_POINTER_OVERFLOW]]:
  // CHECK-SANITIZE-NORECOVER-NEXT:     call void @__ubsan_handle_pointer_overflow_abort(ptr @[[LINE_700]], i64 %[[BASE_RELOADED_INT]], i64 %[[COMPUTED_GEP]])
  // CHECK-SANITIZE-RECOVER-NEXT:       call void @__ubsan_handle_pointer_overflow(ptr @[[LINE_700]], i64 %[[BASE_RELOADED_INT]], i64 %[[COMPUTED_GEP]])
  // CHECK-SANITIZE-TRAP-NEXT:          call void @llvm.ubsantrap(i8 19){{.*}}, !nosanitize
  // CHECK-SANITIZE-UNREACHABLE-NEXT:   unreachable, !nosanitize
  // CHECK-SANITIZE:                  [[CONT]]:
  // CHECK-NEXT:                        store ptr %[[ADD_PTR]], ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        %[[ADD_PTR_RELOADED:.*]] = load ptr, ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        ret ptr %[[ADD_PTR_RELOADED]]
#line 700
  ++base;
  return base;
}

char *predec(char *base) {
  // CHECK:                           define{{.*}} ptr @predec(ptr noundef %[[BASE:.*]])
  // CHECK-NEXT:                      [[ENTRY:.*]]:
  // CHECK-NEXT:                        %[[BASE_ADDR:.*]] = alloca ptr, align 8
  // CHECK-NEXT:                        store ptr %[[BASE]], ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        %[[BASE_RELOADED:.*]] = load ptr, ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        %[[ADD_PTR:.*]] = getelementptr inbounds i8, ptr %[[BASE_RELOADED]], i32 -1
  // CHECK-SANITIZE-NEXT:               %[[BASE_RELOADED_INT:.*]] = ptrtoint ptr %[[BASE_RELOADED]] to i64, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP:.*]] = add i64 %[[BASE_RELOADED_INT]], -1, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BASE_IS_NOT_NULLPTR:.*]] = icmp ne ptr %[[BASE_RELOADED]], null, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP_IS_NOT_NULL:.*]] = icmp ne i64 %[[COMPUTED_GEP]], 0, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[BOTH_POINTERS_ARE_NULL_OR_BOTH_ARE_NONNULL:.*]] = icmp eq i1 %[[BASE_IS_NOT_NULLPTR]], %[[COMPUTED_GEP_IS_NOT_NULL]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[COMPUTED_GEP_IS_ULE_BASE:.*]] = icmp ule i64 %[[COMPUTED_GEP]], %[[BASE_RELOADED_INT]], !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[AND_TRUE:.*]] = and i1 %[[COMPUTED_GEP_IS_UGE_BASE]], true, !nosanitize
  // CHECK-SANITIZE-NEXT:               %[[GEP_IS_OKAY:.*]] = and i1 %[[BOTH_POINTERS_ARE_NULL_OR_BOTH_ARE_NONNULL]], %[[AND_TRUE]], !nosanitize
  // CHECK-SANITIZE-NEXT:               br i1 %[[GEP_IS_OKAY]], label %[[CONT:.*]], label %[[HANDLER_POINTER_OVERFLOW:[^,]+]],{{.*}} !nosanitize
  // CHECK-SANITIZE:                  [[HANDLER_POINTER_OVERFLOW]]:
  // CHECK-SANITIZE-NORECOVER-NEXT:     call void @__ubsan_handle_pointer_overflow_abort(ptr @[[LINE_800]], i64 %[[BASE_RELOADED_INT]], i64 %[[COMPUTED_GEP]])
  // CHECK-SANITIZE-RECOVER-NEXT:       call void @__ubsan_handle_pointer_overflow(ptr @[[LINE_800]], i64 %[[BASE_RELOADED_INT]], i64 %[[COMPUTED_GEP]])
  // CHECK-SANITIZE-TRAP-NEXT:          call void @llvm.ubsantrap(i8 19){{.*}}, !nosanitize
  // CHECK-SANITIZE-UNREACHABLE-NEXT:   unreachable, !nosanitize
  // CHECK-SANITIZE:                  [[CONT]]:
  // CHECK-NEXT:                        store ptr %[[ADD_PTR]], ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        %[[ADD_PTR_RELOADED:.*]] = load ptr, ptr %[[BASE_ADDR]], align 8
  // CHECK-NEXT:                        ret ptr %[[ADD_PTR_RELOADED]]
#line 800
  --base;
  return base;
}

#ifdef __cplusplus
}
#endif
