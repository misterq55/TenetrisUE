# Contributing Guidelines

## Coding Standards

### Comments
- Write all code comments in English.
- Mark future enhancements, incomplete work, or deferred logic with `TODO` comments so they can be easily located later.

Example:
```cpp
// TODO: Extend candidate evaluation to include hold/next piece (shallow search).
// TODO: Support SRS wall-kick reachability for advanced placements (T-spin, etc.).
```

### Naming Conventions
- Classes use the `FTN` prefix for plain C++ classes (matching existing `FTNRecorder`, `FTNFieldModel`, etc.).
- Private functions, local variables, and parameters use `camelCase`.
- Public functions and member variables use `PascalCase`.