#include "synthizer.h"

#include "synthizer/c_api.hpp"
#include "synthizer/context.hpp"
#include "synthizer/memory.hpp"
#include "synthizer/sources.hpp"
#include "synthizer/types.hpp"

#include <array>
#include <algorithm>

namespace synthizer {

void DirectSource::run() {
	/*
	 * For now, use the fact that we know that the context is always stereo.
	 * In future this'll need to be done better.
	 * */
	this->fillBlock(2);
	float *buf = this->context->getDirectBuffer();
	for(unsigned int i = 0; i < config::BLOCK_SIZE * 2; i++) {
		buf[i] += this->block[i];
	}
}

}


using namespace synthizer;

SYZ_CAPI syz_ErrorCode syz_createDirectSource(syz_Handle *out, syz_Handle context) {
	SYZ_PROLOGUE
	auto ctx = fromC<Context>(context);
	auto ret = ctx->createObject<DirectSource>();
	ctx->registerSource(ret);
	*out = toC(ret);
	return 0;
	SYZ_EPILOGUE
}
