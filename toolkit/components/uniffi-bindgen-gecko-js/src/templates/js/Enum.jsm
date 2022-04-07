{%- let enum_ = ci.get_enum_definition(name).unwrap() %}

class {{ enum_.nm() }} {}

EXPORTED_SYMBOLS.push("{{ enum_.nm() }}");

{% for variant in enum_.variants() %}
{{enum_.nm()}}.{{variant.name().to_camel_case() }} = class {
    constructor(
        {% for field in variant.fields() -%}
        {{field.nm()}}{%- if loop.last %}{%- else %}, {%- endif %}
        {% endfor -%}
        ) {
            {%- for field in variant.fields() %}
            this.{{field.nm()}} = {{ field.nm() }};
            {%- endfor %}
        }
}
{%-endfor %}

class {{ ffi_converter }} extends FfiConverterArrayBuffer {
    static read(dataStream) {
        switch (dataStream.readInt32()) {
            {%- for variant in enum_.variants() %}
            case {{ loop.index }}:
                return new {{ enum_.nm() }}.{{ variant.name().to_camel_case()  }}(
                    {%- for field in variant.fields() %}
                    {{ field.ffi_converter() }}.read(dataStream){%- if loop.last %}{% else %}, {%- endif %}
                    {%- endfor %}
                    );
            {%- endfor %}
            default:
                return new Error("Unknown {{ enum_.nm() }} variant");
        }
    }

    static write(dataStream, value) {
        {%- for variant in enum_.variants() %}
        if (value instanceof {{enum_.nm()}}.{{ variant.name().to_camel_case() }}) {
            dataStream.writeInt32({{ loop.index }});
            {%- for field in variant.fields() %}
            {{ field.ffi_converter() }}.write(dataStream, value.{{ field.nm() }});
            {%- endfor %}
            return;
        }
        {%- endfor %}
        return new Error("Unknown {{ enum_.nm() }} variant");
    }

    static computeSize(value) {
        // Size of the Int indicating the variant
        let totalSize = 4;
        {%- for variant in enum_.variants() %}
        if (value instanceof {{enum_.nm()}}.{{ variant.name().to_camel_case() }}) {
            {%- for field in variant.fields() %}
            totalSize += {{ field.ffi_converter() }}.computeSize(value.{{ field.nm() }});
            {%- endfor %}
            return totalSize;
        }
        {%- endfor %}
        return new Error("Unknown {{ enum_.nm() }} variant");
    }
}
