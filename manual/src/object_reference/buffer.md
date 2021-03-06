# Buffer

## Constructors

### `syz_createBufferFromStream`

```
SYZ_CAPI syz_ErrorCode syz_createBufferFromStream(syz_Handle *out, const char *protocol, const char *path, const char *options);
```

Currently, the only way to make a buffer is from a stream, in the self-explanatory manner. See [Streams](../concepts/streams.md) for information on streams.

This call will decode the stream in the calling thread, returning errors as necessary. Synthizer will eventually offer a BufferCache which supports background decoding and caching, but for the moment the responsibility of background decoding is placed on the calling program.

## Properties

None.

## Functions

### Getters

```
SYZ_CAPI syz_ErrorCode syz_bufferGetChannels(unsigned int *out, syz_Handle buffer);
SYZ_CAPI syz_ErrorCode syz_bufferGetLengthInSamples(unsigned int *out, syz_Handle buffer);
SYZ_CAPI syz_ErrorCode syz_bufferGetLengthInSeconds(double *out, syz_Handle buffer);
```

The self-explanatory getters. These aren't properties because they can't be written and they shouldn't participate in the property infrastructure.

## Remarks

Buffers hold audio data, as a collection of contiguous chunks.  Data is resampled to the Synthizer samplerate and converted to 16-bit PCM using triangular dither.

Buffers are one of the few Synthizer objects that don't require a context.  They may be used freely with any object requiring a buffer, from any thread.  In order to facilitate this, buffers are immutable after creation.

The approximate memory usage of a buffer in bytes is `2 * channels * duration_in_seconds * 44100`.  Loading large assets into buffers is not recommended. For things such as music tracks, use [StreamingGenerator](./streaming_generator.md)s.  Note that on 32-bit architectures, some operating systems only allow a 2 gigabyte address space.  Synthizer avoids allocating buffers as contiguous arrays in part to allow efficient use of 32-bit address spaces, but this only goes so far.  If on a 32-bit architecture, expect to run out of memory from Synthizer's perspective well before decoding 2 Gigabytes of buffers simultaneously due to the inability to find consecutive free pages.
