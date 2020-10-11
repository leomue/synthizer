#pragma once
#include "synthizer.h"

#include "synthizer/error.hpp"
#include "synthizer/memory.hpp"
#include "synthizer/property_internals.hpp"

#include <atomic>

namespace synthizer {

/* Forward declare the routing handles. */
namespace router {
	class ReaderHandle;
	class WriterHandle;
}

class Context;

/*
 * The ultimate base class for all Synthizer objects except those which aren't associated with a context.
 * 
 * To use this properly, make sure that you virtual inherit from this directly or indirectly if adding a new object type.
 * 
 * Synthizer does initialization as a two-step process:
 * - First the constructor is called. It should not rely on being
 *   in the audio thread, and may be called from any thread.
 * - Then initInAudioThread is called, which is in the context's audio thread. initInAudioThread should try to avoid allocating if at all possible.
 * 
 * Though we haven't yet, there will eventually be a custom allocator. Once this is introduced, all stl containers should use it, and all non-STL allocation
 * should also go through it or an associated function as well. This will allow all actual deallocation to happen
 * on a low-priority background thread. At this point, there will be little to no trips through the kernel on audio threads.
 * */
class BaseObject: public CExposable {
	public:
	/*
	 * Constructors of derived objects should only allocate, and should otherwise be threadsafe.
	 * */
	BaseObject(const std::shared_ptr<Context> &ctx): context(ctx) {}
	/*
	 * This is called in the audio thread to initialize the object and should try not to allocate.
	 * */
	virtual void initInAudioThread() {}

	virtual ~BaseObject() {}

	/* A baseObject has no properties whatsoever. */
	virtual bool hasProperty(int property) {
		return false;
	}

	virtual property_impl::PropertyValue getProperty(int property) {
		throw EInvalidProperty();
	}

	virtual void validateProperty(int property, const property_impl::PropertyValue &value) {
		throw EInvalidProperty();
	}

	virtual void setProperty(int property, const property_impl::PropertyValue &value) {
		throw EInvalidProperty();
	}

	/* Virtual because context itself needs to override to always return itself. */
	virtual std::shared_ptr<Context> getContext() {
		return this->context;
	}

	/* From the C API implementations, you usually want this one. */
	virtual Context *getContextRaw() {
		return this->context.get();
	}

	/*
	 * Routing. If either of these returns an actual non-nullptr value, this object supports that half
	 * of the routing architecture (i.e. readers are effects, writers are sources).
	 * */
	virtual router::ReaderHandle *getReaderHandle() {
		return nullptr;
	}

	virtual router::WriterHandle *getWriterHandle() {
		return nullptr;
	}

	protected:
	std::shared_ptr<Context> context;
};

}