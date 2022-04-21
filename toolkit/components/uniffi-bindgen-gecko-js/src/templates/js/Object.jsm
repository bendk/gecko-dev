{%- let object = ci.get_object_definition(name).unwrap() %}

class {{ object.nm() }} {
    // Use `init` to instantiate this class.
    // DO NOT USE THIS CONSTRUCTOR DIRECTLY
    constructor(ptr) {
        if (!ptr) {
            throw new UniFFIError("Attempting to construct an object using the JavaScript constructor directly" +
            "Please use a UDL defined constructor, or the init function for the primary constructor")
        }
        this.ptr = ptr;
        this.destroyed = false;
        this.callCounter = 0;
    }

    {%- for cons in object.constructors() %}
    {%- if cons.is_async() %}
    /**
     * An async constructor for {{ object.nm() }}.
     * 
     * @returns {Promise<{{ object.nm() }}>}: A promise that resolves
     *      to a newly constructed {{ object.nm() }}
     */
    {%- else %}
    /**
     * A constructor for {{ object.nm() }}.
     * 
     * @returns { {{ object.nm() }} }
     */
    {%- endif %}
    static {{ cons.nm() }}({{cons.arg_names()}}) {
        {% call js::call_constructor(cons, type_) %}
    }
    {%- endfor %}

    {%- for meth in object.methods() %}
    {{ meth.nm() }}({{ meth.arg_names() }}) {
        return this.runMethod(() => {
            {% call js::call_method(meth, type_, object) %}
        });
    }
    {%- endfor %}

    destroy() {
        this.destroyed = true;
        // If the call counter is not zero, there are ongoing calls that haven't concluded
        // yet. The function calls themselves will make sure to deallocate the object once the last
        // one concludes and we will prevent any new calls by throwing a UniFFIError
        if (this.callCounter === 0) {
            {{ ci.scaffolding_name() }}.{{ object.ffi_object_free().nm() }}(this.ptr);
        }
    }

    runMethod(callback) {
        if (this.destroyed) {
            throw new UniFFIError("Attempting to call method on Object that is already destroyed")
        }
        try {
            this.callCounter += 1;
            return callback();
        } finally {
            this.callCounter -=1;
            if (this.destroyed && this.callCounter === 0) {
                {{ ci.scaffolding_name() }}.{{ object.ffi_object_free().nm() }}(this.ptr);
            }
        }
    }
}

class {{ ffi_converter }} extends FfiConverter {
    static lift(value) {
        return new {{ object.nm() }}(value);
    }

    static lower(value) {
        return value.ptr;
    }

    // Note: We store the object pointer using the `setPrivate` JS API.  From
    // the JS side, this appears as a 64-bit float value.

    static read(dataStream) {
        return this.lift(dataStream.readFloat64());
    }

    static write(dataStream, value) {
        dataStream.writeFloat64(this.lower(value));
    }

    static computeSize(value) {
        return 8;
    }
}

EXPORTED_SYMBOLS.push("{{ object.nm() }}");
