# StreamingGenerator

Inherits from [Generator](./generator.md).

## Constructors

### `syz_createStreamingGenerator`

```
SYZ_CAPI syz_ErrorCode syz_createStreamingGenerator(syz_Handle *out, syz_Handle context, const char *protocol, const char *path, const char *options);
```

Creates a StreamingGenerator from the standard [stream parameters](../concepts/streams.md).

## Properties

Enum | Type | Default Value | Range | Description
--- | --- | --- | --- | ---
SYZ_P_POSITION | double | 0.0 | value >= 0.0 | The position in of the stream.
SYZ_P_LOOPING | int | 0 | 0 or 1 | Whether playback loops

## Remarks

StreamingGenerator plays streams, decoding and reading on demand.  The typical use case is for music playback.

Due to the expense of streaming from disk and other I/O sources, having more than a few StreamingGenerators going will cause a decrease in audio quality on many systems, typically manifesting as drop--outs and crackling.
StreamingGenerator creates one background thread per instance and does all decoding and I/O in that thread.

At startup, StreamingGenerator's background thread eagerly decodes a relatively large amount of data in order to build up a buffer which prevents underruns.
Thereafter, it will pick up property changes every time the background thread wakes up to add more data to the buffer.  This means that most operations are high latency, currently on the order of 100 to 200 MS.
The least latent operation is the initial start-up, which will begin playing as soon as enough data is decoded.  How long that takes depends on the format and I/O characteristics of the stream, as well as
the user's machine and current load of the system.
