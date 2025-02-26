/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

// Define pointer types
{%- for (preprocessor_condition, pointer_types, preprocessor_condition_end) in pointer_types.iter() %}
{{ preprocessor_condition }}
{%- for pointer_type in pointer_types %}
const static mozilla::uniffi::UniFFIPointerType {{ pointer_type.name }} {
  "{{ pointer_type.label }}"_ns,
  {{ pointer_type.ffi_func_clone.0 }},
  {{ pointer_type.ffi_func_free.0 }},
};
{%- endfor %}
{{ preprocessor_condition_end }}
{%- endfor %}

Maybe<already_AddRefed<UniFFIPointer>> ReadPointer(const GlobalObject& aGlobal, uint64_t aId, const ArrayBuffer& aArrayBuff, long aPosition, ErrorResult& aError) {
  const UniFFIPointerType* type;
  switch (aId) {
    {%- for (preprocessor_condition, pointer_types, preprocessor_condition_end) in pointer_types.iter() %}
{{ preprocessor_condition }}
    {%- for pointer_type in pointer_types %}
    case {{ pointer_type.id }}: {
      type = &{{ pointer_type.name }};
      break;
    }
    {%- endfor %}
{{ preprocessor_condition_end }}
    {%- endfor %}
    default:
      return Nothing();
  }
  return Some(UniFFIPointer::Read(aArrayBuff, aPosition, type, aError));
}

bool WritePointer(const GlobalObject& aGlobal, uint64_t aId, const UniFFIPointer& aPtr, const ArrayBuffer& aArrayBuff, long aPosition, ErrorResult& aError) {
  const UniFFIPointerType* type;
  switch (aId) {
    {%- for (preprocessor_condition, pointer_types, preprocessor_condition_end) in pointer_types.iter() %}
{{ preprocessor_condition }}
    {%- for pointer_type in pointer_types %}
    case {{ pointer_type.id }}: {
      type = &{{ pointer_type.name }};
      break;
    }
    {%- endfor %}
{{ preprocessor_condition_end }}
    {%- endfor %}
    default:
      return false;
  }
  aPtr.Write(aArrayBuff, aPosition, type, aError);
  return true;
}
