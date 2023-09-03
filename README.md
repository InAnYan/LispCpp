# Lisp interpreter
This project is written for educational purposes.

## License
MIT

## Language

### Value types
- Integer: `123`, `42`, `13`. 
  Signed integer value.
- Float: `3.14`, `9.8`.
  Double-preicison floating-point value (IEEE 754).
- String: `"abc"`, `"string"`.
  Double-quoted sequence of characters.
- Symbol: `lambda`, `cond`, `x`.
  Sequence of non-whitespace characters (- some other characters).
- Function: lambda or primitive.
- Cons cell.
- NIL: Special kind of value. 
  Used as false value and to mark end of list.

### Evaluation rules
- Self-evaluable types: integer, float, string, function and nil.
- Evaluation of symbols:
  - Retrieve the value of symbol from lexical environment.
  - If there is no binding, then get the global binding of the symbol.
- Evaluation of cons cells (only proper lists are allowed to be evaluated):
  - Evaluate every member of the list.
  - The first member of the list should be a function.
  - Call the first member of the evaluated list with arguments that are the rest of the evaluated list.

### Capabilities
- GC.
- No closures.
- Macros.
- Special variables.
- `&optional` and `&rest` arguments. No keyword arguments.
- Some handling of internal errors with restarts.
- Special forms: `quote`, `if`, `execute`, `lambda`, `macro`, `define`, `let-one`, `macro-expand`, `cond`, `redefine`.
- Primitives: `binary-add`, `binary-substract`, `binary-multiply`, `binary-divide`, `binary-greater`, `binary-less`, `negate`, `%`, `round`, `floor`, `ceil`, `truncate`, `random`, `eq`, `eql`, `apply`, `print`, `print-char`, `print-pretty`, `print-new-line`, `load`, `read-line`, `read`, `read-all`, `first`, `rest`, `set-car`, `set-cdr`, `cons`, `list`, `null-p`, `cons-p`, `list-p`, `callable-p`, `lambda-p`, `macro-p`, `primitive-p`, `integer-p`, `float-p`, `number-p`, `string-p`, `symbol-p`, `eval`, `system-quit`, `symbol-intern`, `internal-get-stack-trace`.
- From prelude: `binary-append`, `define-macro`, `define-function`, `id`, `second`, `third`, `last-p`, `plus-p`, `minus-p`, `nth`, `binary-greater-equal`, `binary-less-equal`, `when`, `unless`, `let`, `print-ln`, `not`, `and`, `or`, `call-function`, `composition`, `reduce`, `reduce1`, `+`, `-`, `*`, `/`, `complement-eql`, `>`, `>=`, `<`, `<=`, `=`, `/=`, `equal`, `map`, `filter`, `copy-list`, `append`, `reverse`, `abs`, `even-p`, `odd-p`, `list-length`, `length`, `alist-get`, `plist-get`, `take`, `drop`, `format`, `quit`, `string-append`.

## Implementation

### Values implementation
Lisp value has a two level representation:
- Class `Lisp::Value` is a sum type between `integer`, `float`, `primitive`, `nil` and pointer to `object`. It is implemented with `std::variant` help.
- Class `Lisp::Object` is a root class for all referencable types and types that hold more than one field or hold variable sized information.
The function type is split into `Lisp::Primitive` and `Lisp::LambdaFunction`, becasue a primitive is a simple C++ function pointer, but lambda holds two fields of `Value` (parameters and body).

### Object management and GC
- The interpreter uses mark-and-sweep GC algorithm. 
- All managed objects should inherit from `Lisp::Object` and be created via `Lisp::MemoryManager::AllocateObject`. 
- Objects form an intrusive singly-linked linked list. 
- All objects contain mark bit and a pointer to the next allocated object.
- The mark stage and deletion are handled by virtual functions.
- The interpreter interns all strings and symbols. But there is a way to make them without interning.

### Code structure
- Namespace `Lisp::Funcs` contains some function that are useful when dealing with Lisp values. These functions don't check types.
- Namespace `Lisp::Primitives` contains primitive functions.
- The class `Lisp::Context` is the main class of the interpreter. It's a class that stores intermediate information for functions that operate over Lisp values.
- You will notice, that there are lots of classes. I decided to split different algorithmic parts of the interpreter into classes that hold reference to `Lisp::Context`, even if they can be implemented as functions that take a `Lisp::Context&` parameter.

### About macros
Because the definition of data and code in Lisp is deeply interconnected, it's natural to Lisp to have different implementations of syntax checking stage, macro expansion stage with evaluation.

I don't understand this part fully, because I haven't read many books about interpreters and Lisp. So for now, I decided to perform macro expansion while evaluating Lisp forms and not to check the correctnes of lambda parameters.
