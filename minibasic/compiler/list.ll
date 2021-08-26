@vars = private global [26 x i64] zeroinitializer
@nl = private constant [2 x i8] c"\0a\00"

declare void @writeInteger(i64)
declare void @writeString(i8*)

declare void @GC_init()
declare i8* @GC_malloc(i64);

%nodetype = type { i64, i64 }

define i32 @main() {
entry:
  call void @GC_init()
  %ptrx = getelementptr inbounds [26 x i64], [26 x i64]* @vars, i32 0, i32 23
  store i64 0, i64* %ptrx
  br label %loop

loop:
  %iter = phi i64 [42, %entry], [%remaining, %body]
  %l = phi i64 [0, %entry], [%l_new, %body]
  %loop_cond = icmp sgt i64 %iter, 0
  br i1 %loop_cond, label %body, label %endfor

body:
  %remaining = sub i64 %iter, 1

  %p = call i8* @GC_malloc(i64 16)
  %n = bitcast i8* %p to %nodetype*
  %h = getelementptr inbounds %nodetype, %nodetype* %n, i32 0, i32 0
  store i64 %iter, i64* %h
  %t = getelementptr inbounds %nodetype, %nodetype* %n, i32 0, i32 1
  store i64 %l, i64* %t
  %l_new = ptrtoint %nodetype* %n to i64

  %x = load i64, i64* %ptrx
  %xplus1 = add i64 %x, 1
  store i64 %xplus1, i64* %ptrx
  call void @writeInteger(i64 %xplus1)
  %nl = getelementptr inbounds [2 x i8], [2 x i8]* @nl, i32 0, i32 0
  call void @writeString(i8* %nl)
  br label %loop

endfor:
  ret i32 0
}
