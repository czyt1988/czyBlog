# 记录一个QScopedPointer forward declared 的编译错误

使用QScopedPointer内包含一个前置声明的变量时，仍然发生一个编译错误

编译错误显示(博主这个类名叫`SASelectRegionEditor`,使用IMPL模式实现)
```
D:\Qt\Qt5.7.0\5.7\mingw53_32\include\QtCore\qscopedpointer.h:57: error: invalid application of 'sizeof' to incomplete type 'SASelectRegionEditorPrivate'
         typedef char IsIncompleteType[ sizeof(T) ? 1 : -1 ];
                                              ^
```

错误定位到Qt源码的这个位置:

```cpp
template <typename T>
struct QScopedPointerDeleter
{
    static inline void cleanup(T *pointer)
    {
        // Enforce a complete type.
        // If you get a compile error here, read the section on forward declared
        // classes in the QScopedPointer documentation.
        typedef char IsIncompleteType[ sizeof(T) ? 1 : -1 ];
        (void) sizeof(IsIncompleteType);

        delete pointer;
    }
};
```

上面注释很明确说了需要进行前置声明，但我明明已经进行了前置声明了，仍然会有这个编译错误：

```cpp
class SASelectRegionEditorPrivate;
///
/// \brief 用于控制选区数据的编辑器
///
class SA_COMMON_UI_EXPORT SASelectRegionEditor : public SAAbstractPlotEditor
{
    Q_OBJECT
    QScopedPointer< SASelectRegionEditorPrivate > d_ptr;
public:
    SASelectRegionEditor(SAChart2D *parent);
    ...
```

明明已经有`class SASelectRegionEditorPrivate;`但还是出现这个编译错误

后来折腾来折腾去，最后发现，居然把**虚析构函数加上就可以编译通过**： 

```cpp
class SASelectRegionEditorPrivate;
///
/// \brief 用于控制选区数据的编辑器
///
class SA_COMMON_UI_EXPORT SASelectRegionEditor : public SAAbstractPlotEditor
{
    Q_OBJECT
    QScopedPointer< SASelectRegionEditorPrivate > d_ptr;
public:
    SASelectRegionEditor(SAChart2D *parent);
    ~SASelectRegionEditor();
    ...
```

原因解决但不明白原理
