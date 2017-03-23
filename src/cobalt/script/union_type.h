// This file was GENERATED by command:
//     pump.py union_type.h.pump
// DO NOT EDIT BY HAND!!!

// Copyright 2015 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef COBALT_SCRIPT_UNION_TYPE_H_
#define COBALT_SCRIPT_UNION_TYPE_H_

// Implementation of IDL union types.
// http://heycam.github.io/webidl/#idl-union
//
// The member types for a given union type are set at compile-time as template
// parameters. Use templated IsType<> functions to check if a certain type is
// the specific type.
// http://heycam.github.io/webidl/#dfn-specific-type
// The template function AsType can be used to return the specific type.
//
// Attempting to instantiate a UnionType with an unsupported type will result
// in a compile-time assert.
//
// Attempting to query or retrieve a type that is not a part of the union will
// result in a compile-time assert.
//
// Each UnionTypeN allows for definition of a union type with N members. The
// template parameters should be the flattened member types of the union:
// http://heycam.github.io/webidl/#dfn-flattened-union-member-types
//
// Per the specification, there should be either 0 or 1 nullable types in union.
// In the case that there is one nullable type, the entire union type should
// be declared as nullable (with base::optional<>). A corollary to this is that
// none of the member types in the UnionTypeN template should be nullable.

#include <iosfwd>
#include <limits>

#include "base/memory/aligned_memory.h"
#include "cobalt/script/union_type_internal.h"

namespace cobalt {
namespace script {

template <typename T1, typename T2>
class UnionType2 {
 public:
  UnionType2() : specific_type_(kUnspecified) {}

  explicit UnionType2(typename internal::UnionTypeTraits<T1>::ArgType arg)
      : specific_type_(kTypeT1) {
    new (storage_.void_data()) T1(arg);
  }
  explicit UnionType2(typename internal::UnionTypeTraits<T2>::ArgType arg)
      : specific_type_(kTypeT2) {
    new (storage_.void_data()) T2(arg);
  }

  UnionType2(const UnionType2& other) {
    ConstructFromOther(other);
  }

  UnionType2& operator=(const UnionType2& other) {
    if (&other != this) {
      Destruct();
      ConstructFromOther(other);
    }
    return *this;
  }

  ~UnionType2() {
    Destruct();
  }

  // Forward these checks to the UnionTypeCheck helper class, which works around
  // being unable to do template specializations in class scope.
  template <typename S>
  bool IsType() const {
    return UnionTypeCheck<S>::IsType(this);
  }
  template <typename S>
  typename internal::UnionTypeTraits<S>::ReturnType AsType() {
    return UnionTypeCheck<S>::AsType(this);
  }
  template <typename S>
  typename internal::UnionTypeTraits<S>::ConstReturnType AsType() const {
    return UnionTypeCheck<S>::AsType(this);
  }

 private:
  // Internal helper class for checking and getting the union's specific type.
  // Only partial class template specializations are allowed in class scope,
  // hence the extra dummy template variable.
  template <typename U, bool = false>
  class UnionTypeCheck {
    // Attempting to query for types that are not part of the union will
    // result in a compile-time error.
    COMPILE_ASSERT(sizeof(U) == 0, UnsupportedType);
  };

  // Specializations of the UnionTypeCheck class for each member type of the
  // union.
  template <bool dummy>
  class UnionTypeCheck<T1, dummy> {
    static bool IsType(const UnionType2<T1, T2>* union_value) {
      return union_value->specific_type_ == kTypeT1;
    }
    static typename internal::UnionTypeTraits<T1>::ReturnType
        AsType(UnionType2<T1, T2>* union_value) {
      return *(union_value->storage_.template data_as<T1>());
    }
    static typename internal::UnionTypeTraits<T1>::ConstReturnType
        AsType(const UnionType2<T1, T2>* union_value) {
      return *(union_value->storage_.template data_as<T1>());
    }
    friend class UnionType2<T1, T2>;
  };

  template <bool dummy>
  class UnionTypeCheck<T2, dummy> {
    static bool IsType(const UnionType2<T1, T2>* union_value) {
      return union_value->specific_type_ == kTypeT2;
    }
    static typename internal::UnionTypeTraits<T2>::ReturnType
        AsType(UnionType2<T1, T2>* union_value) {
      return *(union_value->storage_.template data_as<T2>());
    }
    static typename internal::UnionTypeTraits<T2>::ConstReturnType
        AsType(const UnionType2<T1, T2>* union_value) {
      return *(union_value->storage_.template data_as<T2>());
    }
    friend class UnionType2<T1, T2>;
  };

  enum SpecificType {
    kUnspecified = 0,
    kTypeT1,
    kTypeT2,
  };

  union StorageUnion {
    base::AlignedMemory<sizeof(T1), ALIGNOF(T1)> t1;
    base::AlignedMemory<sizeof(T2), ALIGNOF(T2)> t2;
  };

  void ConstructFromOther(const UnionType2& other) {
    specific_type_ = other.specific_type_;
    switch (specific_type_) {
      case kTypeT1:
        new (storage_.void_data()) T1(other.AsType<T1>());
        break;
      case kTypeT2:
        new (storage_.void_data()) T2(other.AsType<T2>());
        break;
      case kUnspecified:
        // no-op
        break;
    }
  }

  void Destruct() {
    switch (specific_type_) {
      case kTypeT1:
        storage_.template data_as<T1>()->T1::~T1();
        break;
      case kTypeT2:
        storage_.template data_as<T2>()->T2::~T2();
        break;
      case kUnspecified:
        // no-op
        break;
    }
    specific_type_ = kUnspecified;
  }

  base::AlignedMemory<sizeof(StorageUnion), ALIGNOF(StorageUnion)> storage_;
  SpecificType specific_type_;

  // Count the number of numeric types in this union. There can be a max of one.
  // Otherwise, the JS->Cobalt conversion is ambiguous.
  // The spec doesn't seem to describe this limitation, but this is what Blink
  // does.
  static const int kNumNumericTypes =
      (internal::UnionTypeTraits<T1>::is_numeric_type ? 1 : 0) +
      (internal::UnionTypeTraits<T2>::is_numeric_type ? 1 : 0);
  COMPILE_ASSERT(kNumNumericTypes <= 1, AmbiguousUnionTypeConversion);
};

// Needed to instantiate base::optional<UnionTypeN>
template <typename T1, typename T2>
inline std::ostream& operator<<(
    std::ostream& stream, const UnionType2<T1, T2>& union_value) {

  if (union_value.template IsType<T1>()) {
    stream << union_value.template AsType<T1>();
  } else if (union_value.template IsType<T2>()) {
    stream << union_value.template AsType<T2>();
  } else {
    stream << "Undefined union type.";
  }

  return stream;
}

template <typename T1, typename T2, typename T3>
class UnionType3 {
 public:
  UnionType3() : specific_type_(kUnspecified) {}

  explicit UnionType3(typename internal::UnionTypeTraits<T1>::ArgType arg)
      : specific_type_(kTypeT1) {
    new (storage_.void_data()) T1(arg);
  }
  explicit UnionType3(typename internal::UnionTypeTraits<T2>::ArgType arg)
      : specific_type_(kTypeT2) {
    new (storage_.void_data()) T2(arg);
  }
  explicit UnionType3(typename internal::UnionTypeTraits<T3>::ArgType arg)
      : specific_type_(kTypeT3) {
    new (storage_.void_data()) T3(arg);
  }

  UnionType3(const UnionType3& other) {
    ConstructFromOther(other);
  }

  UnionType3& operator=(const UnionType3& other) {
    if (&other != this) {
      Destruct();
      ConstructFromOther(other);
    }
    return *this;
  }

  ~UnionType3() {
    Destruct();
  }

  // Forward these checks to the UnionTypeCheck helper class, which works around
  // being unable to do template specializations in class scope.
  template <typename S>
  bool IsType() const {
    return UnionTypeCheck<S>::IsType(this);
  }
  template <typename S>
  typename internal::UnionTypeTraits<S>::ReturnType AsType() {
    return UnionTypeCheck<S>::AsType(this);
  }
  template <typename S>
  typename internal::UnionTypeTraits<S>::ConstReturnType AsType() const {
    return UnionTypeCheck<S>::AsType(this);
  }

 private:
  // Internal helper class for checking and getting the union's specific type.
  // Only partial class template specializations are allowed in class scope,
  // hence the extra dummy template variable.
  template <typename U, bool = false>
  class UnionTypeCheck {
    // Attempting to query for types that are not part of the union will
    // result in a compile-time error.
    COMPILE_ASSERT(sizeof(U) == 0, UnsupportedType);
  };

  // Specializations of the UnionTypeCheck class for each member type of the
  // union.
  template <bool dummy>
  class UnionTypeCheck<T1, dummy> {
    static bool IsType(const UnionType3<T1, T2, T3>* union_value) {
      return union_value->specific_type_ == kTypeT1;
    }
    static typename internal::UnionTypeTraits<T1>::ReturnType
        AsType(UnionType3<T1, T2, T3>* union_value) {
      return *(union_value->storage_.template data_as<T1>());
    }
    static typename internal::UnionTypeTraits<T1>::ConstReturnType
        AsType(const UnionType3<T1, T2, T3>* union_value) {
      return *(union_value->storage_.template data_as<T1>());
    }
    friend class UnionType3<T1, T2, T3>;
  };

  template <bool dummy>
  class UnionTypeCheck<T2, dummy> {
    static bool IsType(const UnionType3<T1, T2, T3>* union_value) {
      return union_value->specific_type_ == kTypeT2;
    }
    static typename internal::UnionTypeTraits<T2>::ReturnType
        AsType(UnionType3<T1, T2, T3>* union_value) {
      return *(union_value->storage_.template data_as<T2>());
    }
    static typename internal::UnionTypeTraits<T2>::ConstReturnType
        AsType(const UnionType3<T1, T2, T3>* union_value) {
      return *(union_value->storage_.template data_as<T2>());
    }
    friend class UnionType3<T1, T2, T3>;
  };

  template <bool dummy>
  class UnionTypeCheck<T3, dummy> {
    static bool IsType(const UnionType3<T1, T2, T3>* union_value) {
      return union_value->specific_type_ == kTypeT3;
    }
    static typename internal::UnionTypeTraits<T3>::ReturnType
        AsType(UnionType3<T1, T2, T3>* union_value) {
      return *(union_value->storage_.template data_as<T3>());
    }
    static typename internal::UnionTypeTraits<T3>::ConstReturnType
        AsType(const UnionType3<T1, T2, T3>* union_value) {
      return *(union_value->storage_.template data_as<T3>());
    }
    friend class UnionType3<T1, T2, T3>;
  };

  enum SpecificType {
    kUnspecified = 0,
    kTypeT1,
    kTypeT2,
    kTypeT3,
  };

  union StorageUnion {
    base::AlignedMemory<sizeof(T1), ALIGNOF(T1)> t1;
    base::AlignedMemory<sizeof(T2), ALIGNOF(T2)> t2;
    base::AlignedMemory<sizeof(T3), ALIGNOF(T3)> t3;
  };

  void ConstructFromOther(const UnionType3& other) {
    specific_type_ = other.specific_type_;
    switch (specific_type_) {
      case kTypeT1:
        new (storage_.void_data()) T1(other.AsType<T1>());
        break;
      case kTypeT2:
        new (storage_.void_data()) T2(other.AsType<T2>());
        break;
      case kTypeT3:
        new (storage_.void_data()) T3(other.AsType<T3>());
        break;
      case kUnspecified:
        // no-op
        break;
    }
  }

  void Destruct() {
    switch (specific_type_) {
      case kTypeT1:
        storage_.template data_as<T1>()->T1::~T1();
        break;
      case kTypeT2:
        storage_.template data_as<T2>()->T2::~T2();
        break;
      case kTypeT3:
        storage_.template data_as<T3>()->T3::~T3();
        break;
      case kUnspecified:
        // no-op
        break;
    }
    specific_type_ = kUnspecified;
  }

  base::AlignedMemory<sizeof(StorageUnion), ALIGNOF(StorageUnion)> storage_;
  SpecificType specific_type_;

  // Count the number of numeric types in this union. There can be a max of one.
  // Otherwise, the JS->Cobalt conversion is ambiguous.
  // The spec doesn't seem to describe this limitation, but this is what Blink
  // does.
  static const int kNumNumericTypes =
      (internal::UnionTypeTraits<T1>::is_numeric_type ? 1 : 0) +
      (internal::UnionTypeTraits<T2>::is_numeric_type ? 1 : 0) +
      (internal::UnionTypeTraits<T3>::is_numeric_type ? 1 : 0);
  COMPILE_ASSERT(kNumNumericTypes <= 1, AmbiguousUnionTypeConversion);
};

// Needed to instantiate base::optional<UnionTypeN>
template <typename T1, typename T2, typename T3>
inline std::ostream& operator<<(
    std::ostream& stream, const UnionType3<T1, T2, T3>& union_value) {

  if (union_value.template IsType<T1>()) {
    stream << union_value.template AsType<T1>();
  } else if (union_value.template IsType<T2>()) {
    stream << union_value.template AsType<T2>();
  } else if (union_value.template IsType<T3>()) {
    stream << union_value.template AsType<T3>();
  } else {
    stream << "Undefined union type.";
  }

  return stream;
}

template <typename T1, typename T2, typename T3, typename T4>
class UnionType4 {
 public:
  UnionType4() : specific_type_(kUnspecified) {}

  explicit UnionType4(typename internal::UnionTypeTraits<T1>::ArgType arg)
      : specific_type_(kTypeT1) {
    new (storage_.void_data()) T1(arg);
  }
  explicit UnionType4(typename internal::UnionTypeTraits<T2>::ArgType arg)
      : specific_type_(kTypeT2) {
    new (storage_.void_data()) T2(arg);
  }
  explicit UnionType4(typename internal::UnionTypeTraits<T3>::ArgType arg)
      : specific_type_(kTypeT3) {
    new (storage_.void_data()) T3(arg);
  }
  explicit UnionType4(typename internal::UnionTypeTraits<T4>::ArgType arg)
      : specific_type_(kTypeT4) {
    new (storage_.void_data()) T4(arg);
  }

  UnionType4(const UnionType4& other) {
    ConstructFromOther(other);
  }

  UnionType4& operator=(const UnionType4& other) {
    if (&other != this) {
      Destruct();
      ConstructFromOther(other);
    }
    return *this;
  }

  ~UnionType4() {
    Destruct();
  }

  // Forward these checks to the UnionTypeCheck helper class, which works around
  // being unable to do template specializations in class scope.
  template <typename S>
  bool IsType() const {
    return UnionTypeCheck<S>::IsType(this);
  }
  template <typename S>
  typename internal::UnionTypeTraits<S>::ReturnType AsType() {
    return UnionTypeCheck<S>::AsType(this);
  }
  template <typename S>
  typename internal::UnionTypeTraits<S>::ConstReturnType AsType() const {
    return UnionTypeCheck<S>::AsType(this);
  }

 private:
  // Internal helper class for checking and getting the union's specific type.
  // Only partial class template specializations are allowed in class scope,
  // hence the extra dummy template variable.
  template <typename U, bool = false>
  class UnionTypeCheck {
    // Attempting to query for types that are not part of the union will
    // result in a compile-time error.
    COMPILE_ASSERT(sizeof(U) == 0, UnsupportedType);
  };

  // Specializations of the UnionTypeCheck class for each member type of the
  // union.
  template <bool dummy>
  class UnionTypeCheck<T1, dummy> {
    static bool IsType(const UnionType4<T1, T2, T3, T4>* union_value) {
      return union_value->specific_type_ == kTypeT1;
    }
    static typename internal::UnionTypeTraits<T1>::ReturnType
        AsType(UnionType4<T1, T2, T3, T4>* union_value) {
      return *(union_value->storage_.template data_as<T1>());
    }
    static typename internal::UnionTypeTraits<T1>::ConstReturnType
        AsType(const UnionType4<T1, T2, T3, T4>* union_value) {
      return *(union_value->storage_.template data_as<T1>());
    }
    friend class UnionType4<T1, T2, T3, T4>;
  };

  template <bool dummy>
  class UnionTypeCheck<T2, dummy> {
    static bool IsType(const UnionType4<T1, T2, T3, T4>* union_value) {
      return union_value->specific_type_ == kTypeT2;
    }
    static typename internal::UnionTypeTraits<T2>::ReturnType
        AsType(UnionType4<T1, T2, T3, T4>* union_value) {
      return *(union_value->storage_.template data_as<T2>());
    }
    static typename internal::UnionTypeTraits<T2>::ConstReturnType
        AsType(const UnionType4<T1, T2, T3, T4>* union_value) {
      return *(union_value->storage_.template data_as<T2>());
    }
    friend class UnionType4<T1, T2, T3, T4>;
  };

  template <bool dummy>
  class UnionTypeCheck<T3, dummy> {
    static bool IsType(const UnionType4<T1, T2, T3, T4>* union_value) {
      return union_value->specific_type_ == kTypeT3;
    }
    static typename internal::UnionTypeTraits<T3>::ReturnType
        AsType(UnionType4<T1, T2, T3, T4>* union_value) {
      return *(union_value->storage_.template data_as<T3>());
    }
    static typename internal::UnionTypeTraits<T3>::ConstReturnType
        AsType(const UnionType4<T1, T2, T3, T4>* union_value) {
      return *(union_value->storage_.template data_as<T3>());
    }
    friend class UnionType4<T1, T2, T3, T4>;
  };

  template <bool dummy>
  class UnionTypeCheck<T4, dummy> {
    static bool IsType(const UnionType4<T1, T2, T3, T4>* union_value) {
      return union_value->specific_type_ == kTypeT4;
    }
    static typename internal::UnionTypeTraits<T4>::ReturnType
        AsType(UnionType4<T1, T2, T3, T4>* union_value) {
      return *(union_value->storage_.template data_as<T4>());
    }
    static typename internal::UnionTypeTraits<T4>::ConstReturnType
        AsType(const UnionType4<T1, T2, T3, T4>* union_value) {
      return *(union_value->storage_.template data_as<T4>());
    }
    friend class UnionType4<T1, T2, T3, T4>;
  };

  enum SpecificType {
    kUnspecified = 0,
    kTypeT1,
    kTypeT2,
    kTypeT3,
    kTypeT4,
  };

  union StorageUnion {
    base::AlignedMemory<sizeof(T1), ALIGNOF(T1)> t1;
    base::AlignedMemory<sizeof(T2), ALIGNOF(T2)> t2;
    base::AlignedMemory<sizeof(T3), ALIGNOF(T3)> t3;
    base::AlignedMemory<sizeof(T4), ALIGNOF(T4)> t4;
  };

  void ConstructFromOther(const UnionType4& other) {
    specific_type_ = other.specific_type_;
    switch (specific_type_) {
      case kTypeT1:
        new (storage_.void_data()) T1(other.AsType<T1>());
        break;
      case kTypeT2:
        new (storage_.void_data()) T2(other.AsType<T2>());
        break;
      case kTypeT3:
        new (storage_.void_data()) T3(other.AsType<T3>());
        break;
      case kTypeT4:
        new (storage_.void_data()) T4(other.AsType<T4>());
        break;
      case kUnspecified:
        // no-op
        break;
    }
  }

  void Destruct() {
    switch (specific_type_) {
      case kTypeT1:
        storage_.template data_as<T1>()->T1::~T1();
        break;
      case kTypeT2:
        storage_.template data_as<T2>()->T2::~T2();
        break;
      case kTypeT3:
        storage_.template data_as<T3>()->T3::~T3();
        break;
      case kTypeT4:
        storage_.template data_as<T4>()->T4::~T4();
        break;
      case kUnspecified:
        // no-op
        break;
    }
    specific_type_ = kUnspecified;
  }

  base::AlignedMemory<sizeof(StorageUnion), ALIGNOF(StorageUnion)> storage_;
  SpecificType specific_type_;

  // Count the number of numeric types in this union. There can be a max of one.
  // Otherwise, the JS->Cobalt conversion is ambiguous.
  // The spec doesn't seem to describe this limitation, but this is what Blink
  // does.
  static const int kNumNumericTypes =
      (internal::UnionTypeTraits<T1>::is_numeric_type ? 1 : 0) +
      (internal::UnionTypeTraits<T2>::is_numeric_type ? 1 : 0) +
      (internal::UnionTypeTraits<T3>::is_numeric_type ? 1 : 0) +
      (internal::UnionTypeTraits<T4>::is_numeric_type ? 1 : 0);
  COMPILE_ASSERT(kNumNumericTypes <= 1, AmbiguousUnionTypeConversion);
};

// Needed to instantiate base::optional<UnionTypeN>
template <typename T1, typename T2, typename T3, typename T4>
inline std::ostream& operator<<(
    std::ostream& stream, const UnionType4<T1, T2, T3, T4>& union_value) {

  if (union_value.template IsType<T1>()) {
    stream << union_value.template AsType<T1>();
  } else if (union_value.template IsType<T2>()) {
    stream << union_value.template AsType<T2>();
  } else if (union_value.template IsType<T3>()) {
    stream << union_value.template AsType<T3>();
  } else if (union_value.template IsType<T4>()) {
    stream << union_value.template AsType<T4>();
  } else {
    stream << "Undefined union type.";
  }

  return stream;
}

}  // namespace script
}  // namespace cobalt

#endif  // COBALT_SCRIPT_UNION_TYPE_H_
