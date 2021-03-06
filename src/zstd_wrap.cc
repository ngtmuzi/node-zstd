#include <nan.h>
#include <zstd.h>

static NAN_METHOD(CompressSync) {

  if (info.Length() < 1 || !node::Buffer::HasInstance(info[0])) {
    return Nan::ThrowError("First argument should be a buffer");
  }

  v8::Local<v8::Object> buf = info[0]->ToObject();
  char *src = node::Buffer::Data(buf);
  size_t srcSize = node::Buffer::Length(buf);

  size_t dstCapacity = ZSTD_compressBound(srcSize);
  char *dst = (char*)malloc(dstCapacity);
  if (NULL == dst) {
    return Nan::ThrowError("Too large, Out of memory!");
  }

  int compressionLevel = 1;
  if (info.Length() == 2) {
    compressionLevel = info[1]->IsUndefined() ? 0 : info[1]->NumberValue();
  }

  size_t cSize = ZSTD_compress(dst, dstCapacity, src, srcSize, compressionLevel);
  if (ZSTD_isError(cSize)) {
    return Nan::ThrowError("Compress failed!");
  }

  Nan::MaybeLocal<v8::Object> dstBuf = Nan::NewBuffer(dst, cSize);

  return info.GetReturnValue().Set(dstBuf.ToLocalChecked());
}

static NAN_METHOD(DecompressSync) {

  if (info.Length() < 1 || !node::Buffer::HasInstance(info[0])) {
    return Nan::ThrowError("First argument should be a buffer");
  }

  v8::Local<v8::Object> buf = info[0]->ToObject();
  char *src = node::Buffer::Data(buf);
  size_t srcSize = node::Buffer::Length(buf);

  size_t dstCapacity = ZSTD_getDecompressedSize(src, srcSize);
  if (0 == dstCapacity) {
    // When `return==0`, consider data to decompress could have any size.
    return Nan::ThrowError("Compressed size unknown");
  }

  char *dst = (char*)malloc(dstCapacity);
  if (NULL == dst) {
    return Nan::ThrowError("Too large, Out of memory!");
  }

  size_t dSize = ZSTD_decompress(dst, dstCapacity, src, srcSize);
  if (ZSTD_isError(dSize))  {
    return Nan::ThrowError("Decompress failed!");
  }

  Nan::MaybeLocal<v8::Object> dstBuf = Nan::NewBuffer(dst, dSize);

  return info.GetReturnValue().Set(dstBuf.ToLocalChecked());
}

static NAN_METHOD(CompressSyncUsingDict) {

  if (info.Length() < 2 ||
      !node::Buffer::HasInstance(info[0]) ||
      !node::Buffer::HasInstance(info[1])) {
    return Nan::ThrowError("First and second argument should be a buffer");
  }

  v8::Local<v8::Object> dictBuf = info[1]->ToObject();
  char *dict = node::Buffer::Data(dictBuf);
  size_t dictSize = node::Buffer::Length(dictBuf);

  int compressionLevel = 1;
  if (info.Length() == 3) {
    compressionLevel = info[2]->IsUndefined() ? 0 : info[2]->NumberValue();
  }

  ZSTD_CDict *cdict = ZSTD_createCDict(dict, dictSize, compressionLevel);

  v8::Local<v8::Object> inputBuf = info[0]->ToObject();
  char *src = node::Buffer::Data(inputBuf);
  size_t srcSize = node::Buffer::Length(inputBuf);

  size_t dstCapacity = ZSTD_compressBound(srcSize);
  char *dst = (char*)malloc(dstCapacity);
  if (NULL == dst) {
    return Nan::ThrowError("Too large, Out of memory!");
  }

  ZSTD_CCtx* const cctx = ZSTD_createCCtx();
  size_t const cSize = ZSTD_compress_usingDict(cctx, dst, dstCapacity, src, srcSize, cdict, sizeof(cdict), compressionLevel);
  ZSTD_freeCCtx(cctx);
  if (ZSTD_isError(cSize)) {
    return Nan::ThrowError("Compress failed!");
  }

  Nan::MaybeLocal<v8::Object> dstBuf = Nan::NewBuffer(dst, cSize);

  return info.GetReturnValue().Set(dstBuf.ToLocalChecked());
}

static NAN_METHOD(CompressSyncUsingCDict) {

  if (info.Length() < 2 ||
      !node::Buffer::HasInstance(info[0]) ||
      !node::Buffer::HasInstance(info[1])) {
    return Nan::ThrowError("First and second argument should be a buffer");
  }

  v8::Local<v8::Object> dictBuf = info[1]->ToObject();
  char *dict = node::Buffer::Data(dictBuf);
  size_t dictSize = node::Buffer::Length(dictBuf);

  int compressionLevel = 1;
  if (info.Length() == 3) {
    compressionLevel = info[2]->IsUndefined() ? 0 : info[2]->NumberValue();
  }

  ZSTD_CDict *cdict = ZSTD_createCDict(dict, dictSize, compressionLevel);

  v8::Local<v8::Object> inputBuf = info[0]->ToObject();
  char *src = node::Buffer::Data(inputBuf);
  size_t srcSize = node::Buffer::Length(inputBuf);

  size_t dstCapacity = ZSTD_compressBound(srcSize);
  char *dst = (char*)malloc(dstCapacity);
  if (NULL == dst) {
    return Nan::ThrowError("Too large, Out of memory!");
  }

  ZSTD_CCtx* const cctx = ZSTD_createCCtx();
  size_t const cSize = ZSTD_compress_usingDict(cctx, dst, dstCapacity, src, srcSize, cdict, sizeof(cdict), compressionLevel);
  ZSTD_freeCCtx(cctx);
  if (ZSTD_isError(cSize)) {
    return Nan::ThrowError("Compress failed!");
  }

  Nan::MaybeLocal<v8::Object> dstBuf = Nan::NewBuffer(dst, cSize);

  return info.GetReturnValue().Set(dstBuf.ToLocalChecked());
}

static NAN_METHOD(DecompressSyncUsingDict) {

  if (info.Length() < 2 ||
      !node::Buffer::HasInstance(info[0]) ||
      !node::Buffer::HasInstance(info[1])) {
    return Nan::ThrowError("First and second argument should be a buffer");
  }

  v8::Local<v8::Object> dictBuf = info[1]->ToObject();
  char *dict = node::Buffer::Data(dictBuf);
  size_t dictSize = node::Buffer::Length(dictBuf);

  ZSTD_DDict *ddict = ZSTD_createDDict(dict, dictSize);

  v8::Local<v8::Object> inputBuf = info[0]->ToObject();
  char *src = node::Buffer::Data(inputBuf);
  size_t srcSize = node::Buffer::Length(inputBuf);

  size_t dstCapacity = ZSTD_getDecompressedSize(src, srcSize);
  if (0 == dstCapacity) {
    // When `return==0`, consider data to decompress could have any size.
    return Nan::ThrowError("Compressed size unknown");
  }

  char *dst = (char*)malloc(dstCapacity);
  if (NULL == dst) {
    return Nan::ThrowError("Too large, Out of memory!");
  }

  ZSTD_DCtx* dctx = ZSTD_createDCtx();
  size_t dSize = ZSTD_decompress_usingDict(dctx, dst, dstCapacity, src, srcSize, ddict, sizeof(ddict));
  ZSTD_freeDCtx(dctx);
  if (ZSTD_isError(dSize))  {
    return Nan::ThrowError("Decompress failed!");
  }

  Nan::MaybeLocal<v8::Object> dstBuf = Nan::NewBuffer(dst, dSize);

  return info.GetReturnValue().Set(dstBuf.ToLocalChecked());
}

static NAN_METHOD(DecompressSyncUsingCDict) {

  if (info.Length() < 2 ||
      !node::Buffer::HasInstance(info[0]) ||
      !node::Buffer::HasInstance(info[1])) {
    return Nan::ThrowError("First and second argument should be a buffer");
  }

  v8::Local<v8::Object> dictBuf = info[1]->ToObject();
  char *dict = node::Buffer::Data(dictBuf);
  size_t dictSize = node::Buffer::Length(dictBuf);

  ZSTD_DDict *ddict = ZSTD_createDDict(dict, dictSize);

  v8::Local<v8::Object> inputBuf = info[0]->ToObject();
  char *src = node::Buffer::Data(inputBuf);
  size_t srcSize = node::Buffer::Length(inputBuf);

  size_t dstCapacity = ZSTD_getDecompressedSize(src, srcSize);
  if (0 == dstCapacity) {
    // When `return==0`, consider data to decompress could have any size.
    return Nan::ThrowError("Compressed size unknown");
  }

  char *dst = (char*)malloc(dstCapacity);
  if (NULL == dst) {
    return Nan::ThrowError("Too large, Out of memory!");
  }

  ZSTD_DCtx* dctx = ZSTD_createDCtx();
  size_t dSize = ZSTD_decompress_usingDDict(dctx, dst, dstCapacity, src, srcSize, ddict);
  ZSTD_freeDCtx(dctx);
  if (ZSTD_isError(dSize))  {
    return Nan::ThrowError("Decompress failed!");
  }

  Nan::MaybeLocal<v8::Object> dstBuf = Nan::NewBuffer(dst, dSize);

  return info.GetReturnValue().Set(dstBuf.ToLocalChecked());
}

NAN_MODULE_INIT(Init) {
  NAN_EXPORT(target, CompressSync);
  NAN_EXPORT(target, DecompressSync);
  NAN_EXPORT(target, CompressSyncUsingDict);
  NAN_EXPORT(target, CompressSyncUsingCDict);
  NAN_EXPORT(target, DecompressSyncUsingDict);
  NAN_EXPORT(target, DecompressSyncUsingCDict);
}

NODE_MODULE(node_zstd, Init)
