

class FfiConverterString {
    static lift(buf) {
        return this.read(new ArrayBufferDataStream(buf));
    }
    static lower(value) {
        const buffer = new ArrayBuffer(this.computeSize(value));
        const dataStream = new ArrayBufferDataStream(buffer);
        this.write(dataStream, value)
        return buffer;
    }

    static write(dataStream, value) {
        const encoder = new TextEncoder();
        const utf8Arr = encoder.encode(value);
        dataStream.writeUint32(utf8Arr.length);
        for (const byte of utf8Arr) {
            dataStream.writeUint8(byte);
        }
    }

    static read(dataStream) {
        const decoder = new TextDecoder();
        const size = dataStream.readUint32();
        const utf8Arr = new Uint8Array(size);
        for (let i = 0; i < size; i++) {
            utf8Arr[i] = dataStream.readUint8();
        }
        return decoder.decode(utf8Arr);
    }

    static computeSize(value) {
        const encoder = new TextEncoder();
        return 4 + encoder.encode(value).length
    }
}
  
