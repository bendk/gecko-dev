{%- let error = ci.get_error_definition(name).unwrap() %}
{%- let string_type = Type::String %}
{%- let string_ffi_converter = string_type.ffi_converter() %}

{% if error.is_flat() %}
class {{ error.name().to_camel_case() }} extends Error {}
EXPORTED_SYMBOLS.push("{{ error.name().to_camel_case() }}");

{% for variant in error.variants() %}
class {{ variant.name().to_camel_case() }} extends {{ error.name().to_camel_case() }} {
    constructor(message, ...params) {
        super(...params);
        this.message = message;
    }
}
EXPORTED_SYMBOLS.push("{{ variant.name().to_camel_case() }}");
{%-endfor %}

class {{ ffi_converter }} extends FfiConverterArrayBuffer {
    static read(dataStream) {
        switch (dataStream.readInt32()) {
            {%- for variant in error.variants() %}
            case {{ loop.index }}:
                return new {{ variant.name().to_camel_case() }}({{ string_ffi_converter }}.read(dataStream));
            {%- endfor %}
            default:
                return new Error("Unknown {{ error.name().to_camel_case() }} variant");
        }
    }
}
{%- endif %}