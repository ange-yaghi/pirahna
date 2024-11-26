# Piranha Language Handbook

## Table of Contents
* [1 Introduction](#1)
* [2 Syntax Overview](#2)
  * [2.1 Literals](#2.1)
    * [2.1.1 Booleans](#2.1.1)
    * [2.1.2 Integers](#2.1.2)
    * [2.1.3 Strings](#2.1.3)
    * [2.1.4 Floating-point Values](#2.1.4)
  * [2.2 Nodes](#2.2)
    * [2.2.1 Node Definitions](#2.2.1)
    * [2.2.2 Node Instances](#2.2.2)
    * [2.2.3 Default Inputs](#2.2.3)
    * [2.2.4 Node Visibility](#2.2.4)
    * [2.2.5 Immediate Node Instances](#2.2.5)
    * [2.2.6 Nested Nodes](#2.2.6)
    * [2.2.7 Toggle Inputs](#2.2.7)
    * [2.2.8 Output Aliasing](#2.2.8)
    * [2.2.9 Input Type Enforcement](#2.2.9)
    * [2.2.10 Binding to Native Code](#2.2.10)
  * [2.3 Files](#2.3)
    * [2.3.1 Importing Files](#2.3.1)
    * [2.3.2 Import Visibility](#2.3.2)
    * [2.3.3 Name Safety](#2.3.3)
  * [2.4 Execution Model](#2.4)
    * [2.4.1 Introduction](#2.4.1)
    * [2.4.2 Node Evaluation](#2.4.2)
    * [2.4.3 Composite Node Evaluation](#2.4.3)
    * [2.4.4 Out of Order Ports](#2.4.4)
  * [2.5 Comments and Documentation](#2.5)
    * [2.5.1 C/C++ Style Comments](#2.5.1)
    * [2.5.2 Piranha Documentation Tags](#2.5.2)
* [3 C++ API](#3)
  * [3.1 Introduction](#3.1)
  * [3.2 Hello World Compiler](#3.2)
    * [3.2.1 Getting Started](#3.2.1)
    * [3.2.2 Understanding the Compilation Error List](#3.2.2)
    * [3.2.3 Building a Program](#3.2.3)
    * [3.2.4 Channels and Literals](#3.2.4)
    * [3.2.5 Assembly Output](#3.2.5)
    * [3.2.6 Custom Nodes](#3.3.6)
    * [3.2.7 Operations](#3.2.7)
    * [3.2.8 Conversions](#3.2.8)
    * [3.2.9 Advanced Functionality](#3.2.9)
<br/>

# <a name="1"></a>1 Introduction

Piranha is a configurable programming language that allows for easy object-oriented binding to C++ classes. It was designed primarily for use in [MantaRay](https://github.com/ange-yaghi/manta-ray) as a high-speed SDL (scene description language). It is better therefore to think of Piranha as a specification language, although it can perform sequential operations as well.

Most examples given in this manual use a completely hypothetical Piranha language and thus cannot be run. Running examples are given in the [Hello World Compiler Tutorial](#3.2).

For syntax highlighting, check out this project: [Piranha Visual Studio Code Extension](https://github.com/ange-yaghi/piranha-vscode-extension)

# <a name="2"></a>2 Syntax Overview

## <a name="2.1"></a>2.1 Literals

Piranha defines a few fundamental literal types which are automatically parsed. It should be noted that these literals don't have any intrinsic meaning to the compiler and must be given definitions in order to "do" anything. This will be discussed later in this manual.

### <a name="2.1.1"></a>2.1.1 Booleans

Boolean values are specified just as they would be in C/C++ using `true` or `false`.

<pre>
<b>node</b> example {
  <b>output</b> boolean: <b>true</b>;
}
</pre>

### <a name="2.1.2"></a>2.1.2 Integers

Integer values can be specified in either hexadecimal or decimal form. 

<pre>
<b>node</b> example {
  <b>output</b> int0: 0x01;
  <b>output</b> int1: 1;
  <b>output</b> int2: 01;
}
</pre>

### <a name="2.1.3"></a>2.1.3 Strings

String values are written as textual information enclosed in double quotes. Special characters like the newline character `\n` are written as they would be in C. Strings are also automatically concatenated if placed side by side (as in C).

<pre>
<b>node</b> example {
  <b>output</b> string: "Hello " "world!";
}
</pre>

### <a name="2.1.4"></a>2.1.4 Floating-point Values

Floating point values are numbers with a decimal. If the decimal is not present the compiler will treat the number as an integer literal.

<pre>
<b>node</b> example {
  <b>output</b> int: 50;      // Interpreted as an integer
  <b>output</b> float: 50.0;  // Interpreted as a floating point value
}
</pre>

## <a name="2.2"></a>2.2 Nodes

### <a name="2.2.1"></a>2.2.1 Node Definitions

The fundamental building block of any Piranha program is the **node**. A node is a structure which may have inputs, outputs and nested nodes. 

The syntax of a node *template* (hereby called a node definition) generally follows this pattern:

<pre>
<b>node</b> node_name {
  // Inputs/outputs
  // ...
  
  // Nested nodes
  // ...
}
</pre>

A hypothetical example is given below in the form of a simple adder:

<pre>
<b>node</b> adder {
  <b>input</b> left;
  <b>input</b> right;
  <b>output</b> out: left + right;
}
</pre>

Intuitively, this node could be thought of as a virtual "circuit" that accepts two arbitrary input channels and then adds them.

### <a name="2.2.2"></a>2.2.2 Node Instances

A node definition by itself is not particularly useful. Analogous to a C++ class, the node definition must be *instantiated*. Piranha uses a syntax very similar to C++ for instantiating nodes.

To instantiate the adder defined above, a possible approach would be:

<pre>
<b>adder</b> adder_name (
  left: 5.0,
  right: 20.0
)
</pre>

A node instantiated in this way is a **full** instantiation because it has all the following parts:
* All input names (`left`, `right`, etc.)
* A node name (`adder_name`)

Having all these parts is not necessary when instantiating a node. Piranha can infer the input name based on the position of the argument in the argument list. If desired some parameters can be specified using positional notation and others can be specified with the full name. 

In addition to not requiring all inputs be labelled, a node may be instantiated without a name. This is called an **unnamed** node instance. For example, the following is valid:

<pre>
<b>adder</b>(5.0, 20.0)
</pre>

as is the following:

<pre>
<b>adder</b>(left: 5.0, 20.0)
</pre>

**Warning**: Mixing positional and named notation in this way can become unwieldy if used haphazardly. For instance, the following will give a syntax error when compiled:

<pre>
<b>adder</b>(5.0, left: 20.0)
</pre>

This is because the `left` input parameter was set multiple times, once through positional notation and once directly by name.

### <a name="2.2.3"></a>2.2.3 Default Inputs

Defining default values for inputs is done through the following syntax:

<pre>
<b>node</b> adder {
  <b>input</b> left: 5.0;
  <b>input</b> right;
  <b>output</b> out: left + right;
}
</pre>

With the code above, if the `left` parameter is not specified, it will take on the value of 5.0. Piranha also allows for default values to be equal to the value of an input. For example, the following is valid:

<pre>
<b>node</b> adder {
  <b>input</b> left: right;
  <b>input</b> right;
  <b>output</b> out: left + right;
}
</pre>

### <a name="2.2.4"></a>2.2.4 Node Visibility

Access to a node definition can be limited to a single file using node visibility keywords. Valid visibility keywords are `public` and `private`. Nodes are `public` by default. For example:

<pre>
<b>private</b> <b>node</b> adder {
  <b>input</b> left: right;
  <b>input</b> right;
  <b>output</b> out: left + right;
}
</pre>

Since this node is declared as `private` it will only be visible within the file in which it is defined. 

### <a name="2.2.5"></a>2.2.5 Immediate Node Instances

It can sometimes be useful to instantiate a node immediately after declaring it. Rather than write out a full instantiation, one can use the following shorthand:

<pre>
<b>auto</b> <b>node</b> adder {
  <b>input</b> left: 5.0;
  <b>input</b> right: 5.0;
  <b>output</b> out: left + right;
}
</pre>

The `auto` keyword will make the new instance have the same name as the node definition it is instantiating. It should be noted that a node must either have no inputs or have defaults provided for all inputs in order to be automatically instantiated. Otherwise an "unconnected input" error will be thrown by the compiler.

### <a name="2.2.6"></a>2.2.6 Nested Nodes

Nodes can be instantiated inside of other nodes. This can be thought of as aggregating nodes into larger nodes with more complex outputs. For instance, take the following hypothetical node definition:

<pre>
<b>node</b> multiply_add {
 <b>input</b> a;
 <b>input</b> b;
 <b>input</b> c;
 <b>output</b> out: a * b + c;
}

<b>node</b> calculate {
  <b>input</b> a;
  <b>input</b> b;
  <b>input</b> c;
  <b>input</b> scale;
  <b>output</b> out: madd.out * scale;
  
  <b>multiply_add</b> madd(
   a: a,
   b: b,
   c: c
  )
}
</pre>

Using the `.` operator, outputs from a node can be referenced by name. In this case the output of the `calculate` node is the `out` output from the `multiply_add` node nested inside the `calculate` node multiplied by the `scale` input.

Similarly it's also possible to instantiate nodes in an inline fashion like so:

<pre>
<b>node</b> calculate {
  <b>input</b> a;
  <b>input</b> b;
  <b>input</b> c;
  <b>input</b> scale;
  <b>output</b> out: <b>multiply_add</b>(a, b, c).out * scale;
}
</pre>

### <a name="2.2.7"></a>2.2.7 Toggle Inputs

Sometimes you may want to disable a node. This is done using a *toggle* port. The following node has a toggle port attached:

<pre>
<b>node</b> calculate {
  <b>input</b> a;
  <b>input</b> b;
  <b>toggle</b> enable: <b>true</b>;
  <b>output</b> c: a + b;
}
</pre>

Inputs to toggle ports must evaluate to boolean values. If the toggle input for a node evaluates to false, the node is disabled. Furthermore, any node that is dependent upon any output of the disabled node is also disabled.

### <a name="2.2.8"></a>2.2.8 Output Aliasing

It is often convenient to not have to explicitly specify an output by name, especially when a node only has one output. To simplify syntax, *output aliasing* can be used. For example:

<pre>
<b>node</b> calculate {
  <b>input</b> a;
  <b>input</b> b;
  <b>alias</b> <b>output</b> c: a + b;
}

<b>calculate</b>(
  a: <b>calculate</b>(1, 2),
  b: <b>calculate</b>(3, 4)
)
</pre>

In the example above, the calculate node aliases the output called `c`. Nodes with an alias output must have only one output.

### <a name="2.2.9"></a>2.2.9 Input Type Enforcement

If desired, the inputs to a node can be constrained to a type. For example:

<pre>
<b>node</b> calculate {
  <b>input</b> a;
  <b>input</b> b;
  <b>output</b> c: a + b;
}

<b>node</b> post_process {
  <b>input</b> a [<b>calculate</b>];
  <b>input</b> b [<b>calculate</b>];
  <b>output</b> c: a.c + b.c;
}
</pre>

In this exampe, the `post_process` node requires that both inputs be references to a `calculate` instance. This will result in the following behavior:

<pre>
<b>node</b> deception { <b>output</b> c: 10; } 

// Will fail because the input types don't match
<b>post_process</b>(5, 10)

// Will fail even though the "deception" type has the required output "c"
<b>post_process</b>(
  <b>deception</b>(), 
  <b>deception</b>()
)

// Okay
<b>post_process</b> (
  a: <b>calculate</b>(1, 2),
  b: <b>calculate</b>(2, 3)
)
</pre>

### <a name="2.2.10"></a>2.2.10 Binding to Native Code

Through the following notation, a Piranha node definition can be made to point to an underlying C++ type. This type of node definition is called a *builtin type*. When binding to a native type, every input and output must have type enforcement tags present. See [1.2.9](#1.2.9) for details. The following is a hypothetical node definition that aliases a C++ class:

<pre>
<b>node</b> adder => cpp_adder_class {
  <b>input</b> a [<b>float</b>]: 0.0;
  <b>input</b> b [<b>float</b>]: 0.0;
  <b>output</b> c [<b>float</b>];
}
</pre>

The C++ class is referenced through a label `cpp_adder_class`. This reference is established when the language rules are generated (see [3.2.3](#3.2.3)). As can be seen default inputs can be specified for builtin types as well so long as they match the enforced input type.

## <a name="2.3"></a>2.3 Files

### <a name="2.3.1"></a>2.3.1 Importing Files
A file in Piranha is called a *compilation unit*. You can't access node definitions in another compilation unit unless you explicitly import that file. This is done with the following notation:

<pre>
<b>import</b> "filename.pr"
</pre>

All node definitions within this file that are public will now be visible.

### <a name="2.3.2"></a>2.3.2 Import Visibility

Imports are private by default. This means that if file *A* imports file *B*, then by default if another file *C* imports *A*, then the definitions inside *B* will not be available to *C*.

To ensure that node definitions imported from another file are visible to outside files, the import statement must be preceded by the `public` keyword as follows:

`file_a.pr`:
<pre>
<b>public</b> <b>import</b> "file_b.pr"
</pre>

`file_b.pr`:
<pre>
<b>public</b> <b>node</b> pub { ... }
<b>private</b> <b>node</b> priv { ... }
</pre>

`file_c.pr`:
<pre>
// Okay
<b>pub</b>( ... )

// Error: Unresolved node
<b>priv</b>( ... )
</pre>

### <a name="2.3.3"></a>2.3.3 Name Safety

To distinguish between two node definitions with the same name but from different libraries, Piranha provides a feature called *import naming*. To name a file that is being imported, the following syntax is used:

<pre>
<b>import</b> "some_library.pr" <b>as</b> lib
</pre>

To instantiate a node type that is defined in "some_lib.pr" one could then write:

<pre>
lib::<b>some_type</b>( ... )
</pre>

If the type being accessed is within a public import inside `some_lib.pr` then it is still accessed through the library name `lib`. At this time Piranha doesn't support nested namespaces in the same way that C++ does.

## <a name="2.4"></a>2.4 Execution Model

### <a name="2.4.1"></a>2.4.1 Introduction

Piranha differs from other programming languages considerably when it comes to its execution model; that is, the order and manner in which statements are executed. Consider the following simple program:

<pre>
<b>print_to_console</b>(result)
<b>add</b> result(10, 30)
</pre>

For those familiar with any sequential programming language, this program would appear to be written backwards. In fact, it would also be logical to assume that such a program would result in a syntax error since the node *result* is used before it's even instantiated. 

In Piranha, however, such a program is completely valid. All dependencies of a node are *evaluated* before the node itself. So even though the add operation is instantiated after the print node textually, it is evaluated first.

The default execution direction is *down*. For example, consider this program:

<pre>
<b>print_to_console</b>("1")
<b>print_to_console</b>("2")
<b>print_to_console</b>("3")
</pre>

This program will print the numbers in the order printed and Piranha will *guarantee* this order since these nodes don't have any dependencies. 

### <a name="2.4.2"></a>2.4.2 Node Evaluation

When a node "runs" it is called *node evaluation*. The Piranha interpreter will ensure that any node will be evaluated one time only. For example:

<pre>
<b>print_to_console</b>(result)
<b>print_to_console</b>(result + 1)
<b>add</b> result(2, 7)
</pre>

In this example, the node `result` is referenced twice in two `print_to_console` nodes. Despite this it is only evaluated once when the first print node is instantiated.

All dependencies will be evaluated before the node itself. For instance, in the example below the node `a` is executed first followed by `b` and then `c`.

<pre>
<b>add</b> c(a, b)
<b>add</b> b(1, 2)
<b>add</b> a(3, 4)
</pre>

### <a name="2.4.3"></a>2.4.3 Composite Node Evaluation

In previous examples, all nodes were built-in types and so did not have any nested nodes. Evaluation of composite nodes follows the same execution model as described previously with the additional stipulation that inputs are evaluated first and outputs are evaluated last. For example:

<pre>
<b>node</b> example {
  <b>input</b> a: <b>add</b>(1, 2);
  <b>input</b> b;
  <b>output</b> result;

  <b>print_to_console</b>(result)
  <b>add</b> result(a, b)
}

<b>example</b>(b: 5)

</pre>

One could equivalently express the above code in *execution order* as:

<pre>
<b>add</b> node0(1, 2)
<b>add</b> result(node0, 5)
<b>print_to_console</b>(result)
</pre>

The example below further demonstrates how composite node dependencies are treated:

<pre>
<b>node</b> get_value {
  <b>alias</b> <b>output</b> out: 5;

  <b>print_to_console</b>("Returning 5")
}

<b>node</b> example {
  <b>input</b> a;
  <b>input</b> b;
  <b>output</b> result;

  <b>print_to_console</b>("The result is: ")
  <b>print_to_console</b>(result)
  <b>add</b> result(a, b)
}

<b>example</b>(value, 10)
<b>get_value</b> value()

</pre>

The above program outputs:

<pre>
Returning 5
The result is:
15
</pre>

### <a name="2.4.4"></a>2.4.4 Out of Order Ports

When inputs or outputs are dependent upon *each other* in a node, then they will be reordered automatically. Take the example below:

<pre>
<b>node</b> some_dependency {
  <b>alias</b> <b>output</b> out: 5;

  <b>print_to_console</b>("Returning 5")
}

<b>node</b> print_and_return {
  <b>input</b> value;
  <b>alias</b> <b>output</b> out: value;

  <b>print_to_console</b>("Now evaluating a");
}

<b>auto</b> <b>node</b> example {
  <b>input</b> a: <b>print_and_return</b>(b) + 5;
  <b>input</b> b: <b>some_dependency</b>();

  <b>print_to_console</b>(a)
}
</pre>

Clearly the input port `a` is dependent upon `b`, thus, `b` will be executed first followed by `a`. This example will output:

<pre>
Returning 5
Now evaluating a
10
</pre>

## <a name="2.5"></a>2.5 Comments and Documentation

### <a name="2.5.1"></a>2.5.1 C/C++ Style Comments

Piranha provides support for both single line and multiline C/C++ style comments. The following examples are treated as comments and are ignored by the compiler:

<pre>
// This is an inline comment
/* This is a multiline comment
   that must be terminated by: */
</pre>

### <a name="2.5.2"></a>2.5.2 Piranha Documentation Tags

Piranha also provides a structured way of documenting code through documentation tags. Using tags, specific ports or nodes can be clearly documented. These values are not ignored by the compiler but are read in and associated with the objects they are tagging. This data can then be used by the Piranha implementation to automatically generate documentation.

Both ports and nodes can be documented using tags with the following syntax:

<pre>
<b>@doc</b>         "Example node documentation "
<b>@author</b>      "John Piranha"
<b>@custom_tag</b>  "Arbitrary data"
<b>node</b> some_node {
  <b>@doc</b> "This is an input"
  <b>input</b> some_input;

  <b>@doc</b> "This is an output"
  <b>output</b> some_output;
}
</pre>

The tag names have no particular meaning but they must follow the naming rules for ordinary labels in Piranha (ie alphanumeric + '_' and cannot begin with a number) and they must begin with the `@` symbol.

Entire files can also be tagged but this requires slightly different syntax. At any point in a file the following block can be added and all documentation tags therein will be associated with the entire file:

<pre>
<b>module</b> {
  <b>@doc</b>     "This documents the entire file"
  <b>@author</b>  "Neil Piranha"
  <b>@date</b>    "July 16, 1969"
}
</pre>

# <a name="3"></a>3 C++ API

## <a name="3.1"></a>3.1 Introduction
Piranha provides some standard compiler utilities for parsing script files but nodes by themselves don't do anything. Built-in types are needed for Piranha applications to do anything meaningful. This section will cover the steps needed to create a Piranha compiler in tutorial form.

## <a name="3.2"></a>3.2 Hello World Compiler

Some details such as the project setup are omitted here for brevity but can be found in the Visual Studio project and full codebase [here](https://github.com/ange-yaghi/piranha-hello-world-compiler).

### <a name="3.2.1"></a>3.2.1 Getting Started

To include all required Piranha headers use the following include statement:

```C++
#include <piranha.h>
```

A bare minimum compiler (that essentially does nothing but check for basic syntax errors) could be written as follows:

`src/main.cpp`:
```C++
#include <piranha.h>

#include <string>
#include <iostream>

int main() {
  std::string fname = "some_script.mr"

  piranha::Compiler compiler;
  piranha::IrCompilationUnit *unit = compiler.compile(fname);

  const piranha::ErrorList *errorList = compiler.getErrorList();
  if (errorList->getErrorCount() > 0) {
    std::cout << "There were " << errorList->getErrorCount() << " compilation errors\n";
  }
  else {
    std::cout << "Compilation was successful\n";
  }
}
```

### <a name="3.2.2"></a>3.2.2 Understanding the Compilation Error List

Printing only the *number* of errors that occurred (while better than doing nothing) is not the most user-friendly option. Luckily, Piranha provides very detailed syntax error information which can be used however desired. At the time of writing, Piranha does not ship with a standard error printing function. One is provided with the Piranha reference compiler and looks like this:

```C++
void printError(const piranha::CompilationError *err) {
    const piranha::ErrorCode_struct &errorCode = err->getErrorCode();
    std::cout 
      << err->getCompilationUnit()->getPath().getStem() 
      << "(" << err->getErrorLocation()->lineStart << "): error " 
      << errorCode.stage << errorCode.code << ": " << errorCode.info << std::endl;

    piranha::IrContextTree *context = err->getInstantiation();
    while (context != nullptr) {
        piranha::IrNode *instance = context->getContext();
        if (instance != nullptr) {
            std::string instanceName = instance->getName();
            std::string definitionName = (instance->getDefinition() != nullptr) 
              ? instance->getDefinition()->getName() 
              : std::string("<Type Error>");

            std::string formattedName = instanceName.empty()
              ? "<unnamed> " + definitionName
              : instanceName + " " + definitionName;

            std::cout 
              << "       While instantiating: " 
              << instance->getParentUnit()->getPath().getStem() 
              << "(" << instance->getSummaryToken()->lineStart << "): " 
              << formattedName << std::endl;
        }

        context = context->getParent();
    }
}

void printErrorTrace(const piranha::ErrorList *errList) {
    int errorCount = errList->getErrorCount();
    for (int i = 0; i < errorCount; i++) {
        printError(errList->getCompilationError(i));
    }
}
```
We'll use this error reporting function in this tutorial. We can now expand our `main()` function to utilize this new printing facility. The filename is also now provided via user-input rather than being hard-coded.

`src/main.cpp`:
```C++
int main() {

  // ...

  std::string fname;
  std::cout << "Input script: ";
  std::cin >> fname;

  // ...

  const piranha::ErrorList *errorList = compiler.getErrorList();
  if (errorList->getErrorCount() > 0) {
    std::cout << "There were " << errorList->getErrorCount() << " compilation errors\n";
    printErrorTrace(errorList);
  }
  else {
    std::cout << "Compilation was successful\n";
  }
}

```

We can now test some simple Piranha scripts. The following Piranha file will compile without any issues:

`test-scripts/1_simple_adder.mr`:
<pre>
<b>node</b> adder_node {
    <b>input</b> a;
    <b>input</b> b;
    <b>output</b> c: a + b;
}

<b>adder_node</b>(1, 2) 
</pre>

If we introduce some syntax errors like the example below, the compiler will complain about syntax errors:

`test-scripts/2_simple_adder_errors.mr`:
<pre>
<b>node</b> adder_node {
    <b>input</b> a;
    <b>input</b> b;
    <b>output</b> c: a + b;
    <b>output</b> d: fake;
}

<b>adder_node</b>(1, 2, 3) 
</pre>

Attempting to compile this program will give the following output:

<pre>
There were 2 compilation errors
2_simple_adder_errors(8): error R0020: Argument position out of bounds
2_simple_adder_errors(5): error R0050: Unresolved reference
</pre>

### <a name="3.2.3"></a>3.2.3 Building a Program

At this point our compiler is fully capable of checking for most Piranha syntax errors. However, it currently cannot *build* anything. In Piranha terminology the process of building means taking a script and then converting it to instances of C++ classes that can actually execute. As of right now, we have no underlying C++ code. 

In order to specify this code we need to implement a class specifying our *language rules*. In it we will describe what C++ classes Piranha nodes will actually be translated into.

`include/language_rules.h`:
```C++
#ifndef HELLO_WORLD_COMPILER_LANGUAGE_RULES_H
#define HELLO_WORLD_COMPILER_LANGUAGE_RULES_H

#include <piranha.h>

class LanguageRules : public piranha::LanguageRules {
public:
    LanguageRules();
    ~LanguageRules();

protected:
    virtual void registerBuiltinNodeTypes();
};

#endif /* HELLO_WORLD_COMPILER_LANGUAGE_RULES_H */
```

As can be seen, it's a simple class with only a single virtual function. Within this single function, however, is where most of this new scripting language will be specified. The constructor and destructor can be left blank:

`src/language_rules.cpp`:
```C++
#include "../include/language_rules.h"

LanguageRules::LanguageRules() {
    /* void */
}

LanguageRules::~LanguageRules() {
    /* void */
}

void LanguageRules::registerBuiltinNodeTypes() {
    // ...
}
```

We can now begin filling in the `registerBuiltinNodeTypes()` function.

### <a name="3.2.4"></a>3.2.4 Channels and Literals

When translating Piranha code into C++ instances, what is actually *happening* needs to be thought about in more detail. For instance, take the following Piranha code:

<pre>
<b>node</b> some_node {
  <b>output</b> port: 15;
}

<b>node</b> some_other_node {
  <b>input</b> port0;
  <b>input</b> port1;

  // ...
}

<b>some_node</b> a()
<b>some_other_node</b> b(a, a.port)
</pre>

Intuitively, it is easy to see that we are creating two nodes `a` and `b`. We are then feeding the entire node `a` to `port0` of `b` and only its output `port` to `port1` of `b`. While this makes sense, we must be more specific when writing our language rules. We've already assumed that the literal `15` will be translated to some kind of integer type, when in reality that doesn't have to happen at all!

A Piranha implementation could for whatever reason decide that it prefers literal integers to be represented as strings in the underlying C++ code and this would be completely valid. Thus, we have to explicitly specify *channel types*. Think of each input and output port communicating through a channel that can only transmit one type of information. With this in mind, we'll now specify how we want our new compiler to handle integers and integer literals in the `registerBuiltinNodeTypes()` function.

`src/language_rules.cpp`:
```C++
// ...

void LanguageRules::registerBuiltinNodeTypes() {
  registerBuiltinType<piranha::ChannelNode>(
    "int_channel", &piranha::FundamentalType::IntType);   

  registerBuiltinType<piranha::DefaultLiteralIntNode>(
    "literal_int");

  registerLiteralType(piranha::LITERAL_INT, "literal_int");
}
```

It may not seem immediately obvious what was done in the above code, but it's much simpler than it appears. In the first statement we have created a new built-in type and labelled it `int_channel`. Piranha provides a node type called `ChannelNode` which acts as a placeholder for a channel type. The channel type is specified in the second argument to the function.

In the next statement we register another built-in type, this time using the default integer literal node that Piranha defines. We label this type `literal_int`. In the statement after this we tell Piranha that all literal types are to be interpreted using the `piranha::DefaultLiteralIntNode` type.

The only thing left to do is create a Piranha representation for the built-in types we have defined, namely `int_channel` and `literal_int`. If they are not defined then the compiler will have no way of knowing that they exist (or what their interface is) at compile-time. A typical design pattern (used in both *MantaRay* and the Piranha Reference Compiler) is to define all built-in types in a *standard library*. Since this is meant to be a simple compiler, all builtin types will be defined in a single file.

`compiler-lib/compiler_lib.mr`:
<pre>
<b>public</b> <b>node</b> int_channel => <b>int_channel</b> { 
  // Integers are atomic and have no inputs/outputs 
}

<b>public</b> <b>node</b> literal_int => <b>literal_int</b> {
    <b>alias</b> <b>output</b> __out [<b>int_channel</b>];
}
</pre>

### <a name="3.2.5"></a>3.2.5 Assembly Output

Since we've added some built-in types to use, we can now begin building programs using Piranha. In Piranha terminology *building* is different from *compiling*. All syntax errors are discovered in the compilation step. Building is the process of creating the executable structure.

**NOTE - Building a script that has syntax errors will result in undefined behavior**

We need to add some additional code to `main.cpp` in order to construct and run executable structures. After checking for syntax errors we add the following code in `main()`:

```C++
  // ...

  const piranha::ErrorList *errorList = compiler.getErrorList();
  if (errorList->getErrorCount() > 0) {
    std::cout << "There were " << errorList->getErrorCount() << " compilation errors\n";
    printErrorTrace(errorList);
  }
  else {
    std::cout << "Compilation was successful\n";

    // Build the program
    piranha::NodeProgram program;
    unit->build(&program);

    // Run the program
    program.execute();
  }
```

We must also tell the compiler which set of language rules to use. This is achieved by changing the compiler instantiation to the following code:

```C++
  LanguageRules rules;
  piranha::Compiler compiler(&rules);
```

The following program should now build and execute without any compilation errors:

`3_sanity_check.mr`:
<pre>
<b>private</b> <b>import</b> "../compiler-lib/compiler_lib.mr"

<b>auto</b> <b>node</b> test {
    <b>output</b> some_int: 15;
}
</pre>

Of course, this program doesn't actually *do* anything so it would be difficult to tell that it ran successfully. A useful tool provided by Piranha is the ability to write an *assembly* file which lists the actual classes instantiated in a node program. We can incorporate this into `main.cpp` as follows:

```C++
    // ...

    // Write the assembly information
    program.writeAssembly(unit->getPath().getStem() + ".pasm");

    // Run the program
    program.execute();

    // ...
```

When we now compile and execute `3_sanity_check.mr` we see the following file also produced:

`3_sanity_check.pasm`:
```
root {
  test {
    literal_int: "15"
      out { __out: @0; }
  }
}
```

As can be seen, two containers have been created. The `root` container is always present and within it is a container for the `test` node that was created. Within this container we can see the literal integer value with a single output that is labelled `@0`. 

### <a name="3.2.6"></a>3.2.6 Custom Nodes

For this example in particular, it would be useful to print out the value of an integer. We can write our own custom node to do this. The full code for this class is given below:

`include/print_int_node.h`:
```C++
#include <piranha.h>

#include <iostream>

class PrintIntNode : public piranha::Node {
public:
  PrintIntNode() {}
  ~PrintIntNode() {}

protected:
  virtual void _evaluate() {
      // Retrieve data from the integer input
      int data;
      static_cast<piranha::IntValueOutput *>(m_integerInput)->fullCompute((void *)&data);

      std::cout << "Integer value: " << data << std::endl;
  }

  virtual void registerInputs() {
      registerInput(&m_integerInput, "value");
  }

protected:
  piranha::pNodeInput m_integerInput;
};

```

We can now add this class to our language rules as follows:

```C++
  // ...

  registerBuiltinType<PrintIntNode>("print_int");

  // ...
```

After it is added to our language rules, we must also add the node to our standard library:

`compiler_lib.mr`:
<pre>
// ...

<b>public</b> <b>node</b> print_int => <b>print_int</b> {
    <b>input</b> value [<b>int_channel</b>]: 0;
}
</pre>

Note the default value of `0` is specified. This means that if this node is instantiated without any inputs, the `value` input will default to the literal integer value of `0`.

We can see this new class in action by running the following script:

`4_print_int_demo.mr`:
<pre>
<b>private</b> <b>import</b> "../compiler-lib/compiler_lib.mr"

<b>auto</b> <b>node</b> main {
    <b>print_int</b>(1)
    <b>print_int</b>(2)
    <b>print_int</b>(3)
}
</pre>

Console output:
```
Integer value: 1
Integer value: 2
Integer value: 3
```

`4_print_int_demo.pasm`:
```
root {
  main {
    literal_int: "1"
      out { __out: @0; }
    print_int
      in  { value: &0; }
    literal_int: "2"
      out { __out: @1; }
    print_int
      in  { value: &1; }
    literal_int: "3"
      out { __out: @2; }
    print_int
      in  { value: &2; }
  }
}
```

As shown in the assembly output, each literal is created with the `__out` port given a unique label and then connected to the input of a `print_int` node.

### <a name="3.2.7"></a>3.2.7 Operations

Piranha already has simple implementations of arithmetic operations like adding and subtracting. We can add an operation to our language rules as follows:

```C++

  // ...

  registerBuiltinType<
    piranha::OperationNodeSpecialized<piranha::native_int, piranha::AddOperationNodeOutput>>("int_add");

  // ...

  registerOperator(
    { piranha::IrBinaryOperator::ADD, &piranha::FundamentalType::IntType, &piranha::FundamentalType::IntType },
    "int_add"
);

```

The `registerOperator(...)` command takes two arguments; the first is a description of the operation and the second is the built-in type that will implement the operation. The descriptor is a structure with the following format:

```C++
struct OperatorMapping {
    IrBinaryOperator::OPERATOR op;
    const ChannelType *leftType;
    const ChannelType *rightType;
    bool reversible = true;
}
```

* `op` - The operator itself (ie. `-`, `+`, `*`, etc.)
* `leftType` - The required type of the left argument
* `rightType` - The required type of the right argument
* `reversible` - Flag to indicate whether the left and right arguments can be reversed

The new operator we've defined above expects two integer type values and implements the `+` operator. We can now update `compiler_lib.mr` with this new operation.

`compiler_lib.mr`:
<pre>
<b>public</b> <b>node</b> int_add => <b>int_add</b> {
    <b>input</b> __left [<b>int_channel</b>];
    <b>input</b> __right [<b>int_channel</b>];
    <b>alias</b> <b>output</b> __out [<b>int_channel</b>];
}
</pre>

`5_integer_add.pasm`:
```
root {
  main {
    literal_int: "1"
      out { __out: @0; }
    literal_int: "1"
      out { __out: @1; }
    int_add
      in  { __in0: &0; __in1: &1; }
      out { __out: @2; }
    print_int
      in  { value: &2; }
    literal_int: "2"
      out { __out: @3; }
    literal_int: "400"
      out { __out: @4; }
    int_add
      in  { __in0: &3; __in1: &4; }
      out { __out: @5; }
    print_int
      in  { value: &5; }
  }
}
```

The compiler has automatically created `int_add` nodes wherever the `+` operator was found (taking two integers as parameters). 

### <a name="3.2.8"></a>3.2.8 Conversions

The `LanguageRules` object provides a function called `registerConversion(...)` which operates in much the same way as the previous functions we've used when defining our language rules. To define a string to integer automatic conversion we first need to define the string channel type, string literal type and then register the node required to make the conversion.

```C++
  // ...

  registerBuiltinType<piranha::ChannelNode>(
    "string_channel", &piranha::FundamentalType::StringType);

  // ...

  registerBuiltinType<piranha::DefaultLiteralStringNode>("literal_string"); 

  // ...

  registerBuiltinType<piranha::StringToIntConversion>("string_to_int");

  // ...

  registerLiteralType(piranha::LITERAL_STRING, "literal_string");

  // ...

  registerConversion({ &piranha::FundamentalType::StringType, &piranha::FundamentalType::IntType }, "string_to_int");

```

This will instruct the compiler to automatically instantiate `piranha::StringToIntConversion` whenever a string must be converted to an integer. As before, we must have the same structures reflected in `compiler_lib.mr`:

`compiler_lib.mr`:
<pre>
<b>public</b> <b>node</b> string_channel => <b>string_channel</b> {
    /* void */
}

// ...

<b>public</b> <b>node</b> literal_string => <b>literal_string</b> {
    <b>alias</b> <b>output</b> __out [<b>string_channel</b>];
}

// ...

<b>public</b> <b>node</b> string_to_int => <b>string_to_int</b> {
    <b>input</b> __in [<b>string_channel</b>];
    <b>alias</b> <b>output</b> __out [<b>int_channel</b>];
}
</pre>

We can see the automatic conversion happen in the following Piranha code:

`6_string_to_int.mr`:
<pre>
<b>private</b> <b>import</b> "../compiler-lib/compiler_lib.mr"

<b>node</b> convert_to_int {
    <b>input</b> value [<b>int_channel</b>];
    <b>alias</b> <b>output</b> converted: value;
}

<b>auto</b> <b>node</b> main {
    <b>convert_to_int</b> s("5")

    <b>print_int</b>(s + 1)
}
</pre>

`6_string_to_int.pasm`:
```
root {
  main {
    literal_string: "5"
      out { __out: @0; }
    s {
      string_to_int
        in  { __in: &0; }
        out { __out: @1; }
    }
    literal_int: "1"
      out { __out: @2; }
    int_add
      in  { __in0: &1; __in1: &2; }
      out { __out: @3; }
    print_int
      in  { value: &3; }
  }
}
```

### <a name="3.2.9"></a>3.2.9 Advanced Functionality
The Piranha Reference Compiler and *MantaRay* implement much more sophisticated compilers than covered in this basic tutorial. For details on the creation of more complex channels and operations please check out these projects.

* [Piranha Reference Compiler](https://github.com/ange-yaghi/piranha/blob/master/demos/reference_language_rules.cpp)
* [MantaRay](https://github.com/ange-yaghi/manta-ray)
