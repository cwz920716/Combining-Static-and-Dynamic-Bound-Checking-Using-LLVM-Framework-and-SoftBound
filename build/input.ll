; ModuleID = 'build/input.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [35 x i8] c"alloca %p [%ld x i%ld], align %ld\0A\00", align 1
@.str1 = private unnamed_addr constant [28 x i8] c"call @malloc %p [%ld x i8]\0A\00", align 1
@x = common global i32 0, align 4

; Function Attrs: alwaysinline nounwind uwtable
define weak void @__track_stack_allocation(i8* %ptr, i64 %array, i64 %stride, i64 %alignment) #0 {
entry:
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([35 x i8]* @.str, i32 0, i32 0), i8* %ptr, i64 %array, i64 %stride, i64 %alignment)
  ret void
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: alwaysinline nounwind uwtable
define weak void @__track_heap_allocation(i8* %ptr, i64 %size) #0 {
entry:
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([28 x i8]* @.str1, i32 0, i32 0), i8* %ptr, i64 %size)
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @f(i32 %s) #2 {
entry:
  %conv = sext i32 %s to i64
  %mul = mul i64 16, %conv
  %call = call noalias i8* @malloc(i64 %mul) #4
  ret i32 %s
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #3

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #2 {
entry:
  %A = alloca [10 x i32], align 16
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %sum.0 = phi i32 [ 0, %entry ], [ %add, %for.inc ]
  %i.0 = phi i32 [ 0, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, 10
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %call = call i32 @f(i32 %i.0)
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [10 x i32]* %A, i32 0, i64 %idxprom
  store i32 %call, i32* %arrayidx, align 4
  %idxprom1 = sext i32 %i.0 to i64
  %arrayidx2 = getelementptr inbounds [10 x i32]* %A, i32 0, i64 %idxprom1
  %tmp = load i32* %arrayidx2, align 4
  %add = add nsw i32 %sum.0, %tmp
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %call3 = call i32 @f(i32 %sum.0)
  ret i32 %call3
}

attributes #0 = { alwaysinline nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.1 (tags/RELEASE_351/final)"}
