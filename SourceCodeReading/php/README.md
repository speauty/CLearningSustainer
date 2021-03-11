## PHP源代码阅读(7.3.25)

#### 载入源码

- 下载源码, 过程省略
- 工具`Clion`

#### 数据结构

##### `zend_refcounted` 

```c
// 引用计数类型, 8字节
// Zend/zend_types.h line 89
typedef struct _zend_refcounted zend_refcounted;
// Zend/zend_types.h line 218
// 定义引用计数类型结构 主要是个zend_refcounted_h的gc头字段
struct _zend_refcounted {
	zend_refcounted_h gc;
};
// Zend/zend_types.h line 211
// 8字节
typedef struct _zend_refcounted_h {
    // 计数 无符号整型-32bit(uint32_t) 4字节
	uint32_t         refcount;
    // 类型信息 联合体 长度为最大字段的长度 也就是32bit 4字节
	union {
		uint32_t type_info;
	} u;
} zend_refcounted_h;

// Zend/zend_types.h line 382
// 常规数据类型
// 未定义标识
#define IS_UNDEF					0
// NULL标识
#define IS_NULL						1
// 布尔false标识
#define IS_FALSE					2
// 布尔true标识
#define IS_TRUE						3
// 长整型标识
#define IS_LONG						4
// 双精度标识
#define IS_DOUBLE					5
// 字符串标识
#define IS_STRING					6
// 树组标识
#define IS_ARRAY					7
// 对象标识
#define IS_OBJECT					8
// 对象标识
#define IS_RESOURCE					9
// 参考标识(内部使用)
#define IS_REFERENCE				10

// 常量类型
// 常量AST标识
#define IS_CONSTANT_AST				11

// 内部类型
// 间接标识
#define IS_INDIRECT             	13
// 指针标识
#define IS_PTR						14
// 错误标识
#define _IS_ERROR					15

// 伪类型, 仅用于类型提示(Z_TYPE(zv)无法使用)
// 布尔标识
#define _IS_BOOL					16
// 可调用标识
#define IS_CALLABLE					17
// 不可数标识
#define IS_ITERABLE					18
// 空标识
#define IS_VOID						19
// 数字标识
#define _IS_NUMBER					20
```

##### `zend_string`

```c
// 字符串类型
// Zend/zend_types.h line 90
typedef struct _zend_string     zend_string;
// Zend/zend_types.h line 222
// 定义zend字符串结构体 25字节 因字节对齐(8bit对齐)后 实际大小为32字节
struct _zend_string {
    // 引用计数字段gc 8字节
	zend_refcounted_h gc;
    // Zend/zend_long.h line 32
    // typedef uint64_t zend_ulong;
    // 记录字符串的hash值 8字节
	zend_ulong        h;
    // 字符串长度 无符号整型 8字节
    // size_t 在32位系统上定义为unsigned int，在64位系统上定义为unsigned long 这里好像做了统一?
	size_t            len;
    // 字符串值的存储位置 柔性树组 C struct hack 内存优化 1字节
    // struct hack 是一种用来实现变长数组的常见技巧, 不必再构造时就确定数组的长度, 延迟到运行时确定
    // 对于struct hack, 因为只需要使用一次malloc分配内存, 所以得到的内存空间是连续的
    // 基本上, 它允许引擎为 zend_string 结构和要存储的字符分配空间, 作为一个单独的 C 指针. 而不是分配两块不连续的空间
	char              val[1];
};
```

##### `zval`

```c
zval 变量类型
// Zend/zend_types.h line 162
// zend_value 结构体 8字节
typedef union _zend_value {
    // 长整型
	zend_long         lval;
    // 双精度型
	double            dval;
    // 引用型
	zend_refcounted  *counted;
    // 字符串型
	zend_string      *str;
    // 树组型
	zend_array       *arr;
    // 对象型
	zend_object      *obj;
    // 资源型
	zend_resource    *res;
    // 参考型
	zend_reference   *ref;
    // ast_ref类型
	zend_ast_ref     *ast;
    // zval类型
	zval             *zv;
	void             *ptr;
    // 类型
	zend_class_entry *ce;
    // 函数型
	zend_function    *func;
	struct {
		uint32_t w1;
		uint32_t w2;
	} ww;
} zend_value;
// zval 结构体 16字节
struct _zval_struct {
    // 变量值 8字节
	zend_value        value;
  	// u1联合体 4字节
	union {
        // 4字节
		struct {
            // Zend/zend_types.h line 42
            // # define ZEND_ENDIAN_LOHI_3(lo, mi, hi)    lo; mi; hi;
            // 采用宏定义 保证结构是按照当前端模式展开
			ZEND_ENDIAN_LOHI_3(
                // 变量类型 1字节
				zend_uchar    type,
                // 变量标识 1字节
                // Zend/zend_types.h 526
                // #define IS_TYPE_REFCOUNTED (1<<0) // 是否需要引用计数类型
				zend_uchar    type_flags,
                // 联合体 2字节
				union {
                    // EX(This)的调用信息 2字节
					uint16_t  call_info;
                    // 保留字段 2字节
					uint16_t  extra;
				} u)
		} v;
        // 类型信息 4字节
		uint32_t type_info;
	} u1;
    // u2联合体  4字节
	union {
        // 哈希冲突链 用来解决哈希冲突 4字节
		uint32_t     next;
        // 运行时缓存 4字节
		uint32_t     cache_slot;
        // opline number (for FAST_CALL) 4字节
		uint32_t     opline_num;
        // 对ast节点的行号 4字节
		uint32_t     lineno;
        // EX(This)的参数个数 4字节
		uint32_t     num_args; 
        // foreach位置 4字节
		uint32_t     fe_pos;
        // foreach游标的标记 4字节
		uint32_t     fe_iter_idx;
        // 类常量访问标识 4字节
		uint32_t     access_flags;
        // 单一属性保护 4字节
		uint32_t     property_guard;
        // 常量类型的标记 4字节
		uint32_t     constant_flags;
        // 保留字段 4字节
		uint32_t     extra;
	} u2;
};
```



##### `zend_arry`

```c
// 树组类型
// Zend/zend_types.h line 91
typedef struct _zend_array      zend_array;

// 数据桶 56字节
typedef struct _Bucket {
    // zval类型的值 16字节
	zval              val;
    // 字符串索引对应的哈希索引或数字索引 8字节
	zend_ulong        h;
    // 字符串值或NULL(如果是数字索引的话) 32字节
	zend_string      *key;
} Bucket;
// 将树组别名记录哈希表
typedef struct _zend_array HashTable;

// Zend/zend_types.h line 237
struct _zend_array {
    // 引用计数字段gc 8字节
	zend_refcounted_h gc;
    // 联合体 4字节
	union {
        // v结构体 4字节
		struct {
			ZEND_ENDIAN_LOHI_4(
                // 哈希表的标记
                // Zend/zend_hash.h line 36
                // 是否一致连续
                // #define HASH_FLAG_CONSISTENCY ((1<<0) | (1<<1))
                // 是否紧密
                // #define HASH_FLAG_PACKED (1<<2)
                // 是否初始化
                // #define HASH_FLAG_INITIALIZED (1<<3)
                // 哈希表key是否为长整型或内部字符串
                // #define HASH_FLAG_STATIC_KEYS (1<<4)
                // 是否存在空的间接val
                // #define HASH_FLAG_HAS_EMPTY_IND    (1<<5)
                // 是否允许违反写时复制
                // #define HASH_FLAG_ALLOW_COW_VIOLATION (1<<6)
				zend_uchar    flags,
                // 未使用的?
				zend_uchar    _unused,
                // 迭代器计数 记录当前哈希表的迭代器的数量
				zend_uchar    nIteratorsCount,
                // 未使用的?
				zend_uchar    _unused2)
		} v;
        // 标记 4字节
		uint32_t flags;
	} u;
    // 掩码 一般为-nTableSize 8字节
	uint32_t          nTableMask;
    // 实际存储容器 存储bucket树组 在一段连续的内存 56字节
	Bucket           *arData;
    // 已使用桶的数量 8字节
	uint32_t          nNumUsed;
    // 有效桶的数量 总是小于或等于nNumUsed 8字节
	uint32_t          nNumOfElements;
    // 哈希表大小 这里表示bucket数量 该字段取值始终是2^n, 最小值是8 8字节
	uint32_t          nTableSize;
    // 哈希表的全局默认游标 8字节
	uint32_t          nInternalPointer;
    // HashTable的自然key 8字节
    // 自然key是指HashTable的应用语义是纯数组时, 插入元素无须指定key, key会以nNextFreeElement的值为准
    // 初始值为0
	 zend_long         nNextFreeElement;
    // 析构函数
    // 当bucket元素被更新或者被删除时, 会对bucket的value调用该函数
    // 如果value是引用计数的类型, 那么会对value引用计数减1, 进而引发可能的gc
    // typedef void (*dtor_func_t)(zval *pDest);
	dtor_func_t       pDestructor;
};

```

##### `zend_class`

```c
// 类迭代器结构
typedef struct _zend_class_iterator_funcs {
	union _zend_function *zf_new_iterator;
	union _zend_function *zf_valid;
	union _zend_function *zf_current;
	union _zend_function *zf_key;
	union _zend_function *zf_next;
	union _zend_function *zf_rewind;
} zend_class_iterator_funcs;

// 对象迭代器结构
typedef struct _zend_object_iterator_funcs {
	// 释放当前迭代器的所有资源
	void (*dtor)(zend_object_iterator *iter);
	// 检测
	int (*valid)(zend_object_iterator *iter);
	// 获取当前元素数据
	zval *(*get_current_data)(zend_object_iterator *iter);
	// 获取当前元素KEY
	void (*get_current_key)(zend_object_iterator *iter, zval *key);
	// 偏移到下一个元素
	void (*move_forward)(zend_object_iterator *iter);
	// 重置
	void (*rewind)(zend_object_iterator *iter);
	// 当前数据或KEY无效
	void (*invalidate_current)(zend_object_iterator *iter);
} zend_object_iterator_funcs;

// 对象迭代器
struct _zend_object_iterator {
    // 对象
	zend_object std;
    // 数据
	zval data;
    // 迭代器
	const zend_object_iterator_funcs *funcs;
	zend_ulong index;
};

// trait方法标准
typedef struct _zend_trait_method_reference {
    // 方法名
	zend_string *method_name;
    // 类名称
	zend_string *class_name;
} zend_trait_method_reference;

// trait优先队列结构
typedef struct _zend_trait_precedence {
    // trait方法
	zend_trait_method_reference trait_method;
    // 包含数量
	uint32_t num_excludes;
    // 包含的类名称(应该是指定某种顺序的)
	zend_string *exclude_class_names[1];
} zend_trait_precedence;

// trait别名记录结构
typedef struct _zend_trait_alias {
    // 方法
	zend_trait_method_reference trait_method;
	// 被加载时的别名记录
	zend_string *alias;
	// 要在trait方法上设置的修饰符?
	uint32_t modifiers;
} zend_trait_alias;

// 类类型
// Zend/zend.h line 110
struct _zend_class_entry {
    // 类的类型 只有两种 1字节
    // 内置类
    // #define ZEND_INTERNAL_CLASS 1
    // 用户自定义类
	// #define ZEND_USER_CLASS 2
	char type;
    // 类名称 32字节
	zend_string *name;
    // 继承的父类指针
	struct _zend_class_entry *parent;
    // 引用计数 4字节
	int refcount;
    // 位组合标记 4字节
	uint32_t ce_flags;
	// 默认普通属性个数
	int default_properties_count;
    // 默认静态属性个数
	int default_static_members_count;
    // 默认普通属性值数组
	zval *default_properties_table;
    // 默认静态属性值数组
	zval *default_static_members_table;
    // 静态属性成员
	zval *static_members_table;
    // 函数哈希表
	HashTable function_table;
    // 类属性信息
	HashTable properties_info;
    // 类常量哈希表
	HashTable constants_table;
	// 构造方法
	union _zend_function *constructor;
    // 析构方法
	union _zend_function *destructor;
    // 克隆方法
	union _zend_function *clone;
	union _zend_function *__get;
	union _zend_function *__set;
	union _zend_function *__unset;
	union _zend_function *__isset;
	union _zend_function *__call;
	union _zend_function *__callstatic;
	union _zend_function *__tostring;
	union _zend_function *__debugInfo;
    // 对象序列化方法
	union _zend_function *serialize_func;
    // 对象反序列化方法
	union _zend_function *unserialize_func;
    
    // 迭代函数指针 仅当类实现Iterator或IteratorAggregate接口时分配
	zend_class_iterator_funcs *iterator_funcs_ptr;

	union {
        // 实例化对象时调用的方法, 默认为函数zend_objects_new, 可以通过扩展或修改源码来改变此值
		zend_object* (*create_object)(zend_class_entry *class_type);
        // 实现该接口的类数量?
		int (*interface_gets_implemented)(zend_class_entry *iface, zend_class_entry *class_type);
	};
    // 迭代器指针
	zend_object_iterator *(*get_iterator)(zend_class_entry *ce, zval *object, int by_ref);
    // 获取静态方法函数指针
	union _zend_function *(*get_static_method)(zend_class_entry *ce, zend_string* method);

	// 序列化方法回调指针
	int (*serialize)(zval *object, unsigned char **buffer, size_t *buf_len, zend_serialize_data *data);
    // 反序列化方法回调指针
	int (*unserialize)(zval *object, zend_class_entry *ce, const unsigned char *buf, size_t buf_len, zend_unserialize_data *data);
	// 类implements的接口个数
	uint32_t num_interfaces;
    // 类use的特性个数
	uint32_t num_traits;
    // 类implements的接口指针
	zend_class_entry **interfaces;
	// 类use的traits指针
	zend_class_entry **traits;
    // 类use的特性方法的别名
	zend_trait_alias **trait_aliases;
    // 类use的特性方法的优先级(用于多个特性有相同名称的方法时, 解决冲突)
	zend_trait_precedence **trait_precedences;
	
    // 记录类的其他信息
	union {
		struct {
            // 所在文件
			zend_string *filename;
            // 开始行号
			uint32_t line_start;
            // 结束行号
			uint32_t line_end;
            // 文档注释
			zend_string *doc_comment;
		} user;
		struct {
			const struct _zend_function_entry *builtin_functions;
			struct _zend_module_entry *module;
		} internal;
	} info;
};
```



##### `zend_object`

```c
// 对象类型
// Zend/zend_types.h line 356
struct _zend_object {
    // 引用计数字段gc头 8字节
	zend_refcounted_h gc;
    // 可能需要移除?
	uint32_t          handle;
    // typedef struct _zend_class_entry     zend_class_entry;
	zend_class_entry *ce;
	const zend_object_handlers *handlers;
    // 属性哈希表 存储对象的动态普通属性值
	HashTable        *properties;
    // 柔性数组, 存储对象的普通属性值
    // 在初始化时创建, 数组大小为对象所属类的默认普通属性个数 default_properties_count+1
	zval              properties_table[1];
};

```

