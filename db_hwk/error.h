#pragma once
//文件说明
//这个文件声明了所有错误处理类

//修改阶数的值时检查的错误
class keyNumError{};
//修改叶节点长度的错误
class itemNumError{};
//试图访问interiorNode的data区域
class dataError{};
//leaf和interior的virtual本身访问错误
class nodeTypeError{};