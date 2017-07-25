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
//key和value大小超过9位
class  outOfRange {};
//缓存的数据大小错误
class cacheLengthError{};
//写缓存的命令错误
class operationError{};
//读取文件错误
class fileError{};
//其他错误
class unexpectedError{};