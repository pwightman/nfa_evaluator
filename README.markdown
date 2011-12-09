## NFA Evaluation

This evaluator supports building and traversing NFA's based on Regular Expressions that support the following actions:

* a + b (union)
* ab (concatenation)
* a* (Kleene star)

### General Usage

```c++
// NFA representing the RE: ((a + b)c)*

Nfa* a = Nfa::simple("a");
Nfa* b = Nfa::simple("b");
Nfa* c = Nfa::simple("c");

a->union(b);
a->concatenate(c);
a->star();

// Run strings on the Nfa

a->isValidString("aaaaabbc", false); // returns true
a->isValidString("aabbcc", false);   // returns true
a->isValidString("xyz", false);      // returns false
```

The booleans being passed into `isValidString` determine whether the evaluation should occur in parallel or not.
_NOTE_: Parallel evaluation is currently broken