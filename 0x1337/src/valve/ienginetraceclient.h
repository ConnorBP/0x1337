#pragma once
#include "../util/memory.h"

#include "centity.h"
#include "cvector.h"

class CRay
{
public:
	constexpr CRay(const CVector& src, const CVector& dst) noexcept
	{
		start = src;
		delta = dst - src;
		isSwept = delta.x || delta.y || delta.z;
	}

private:
	CVector start;
	char pad0[4]{ };
	CVector delta;
	char pad1[40]{ };
	bool isRay{ true };
	bool isSwept;
};

class CTraceFilter
{
public:
	constexpr CTraceFilter(const CEntity* entity) noexcept
	{
		skip = entity;
	}

	virtual bool ShouldHit(CEntity* entity, std::int32_t mask)
	{
		return entity != skip;
	}

	virtual std::int32_t GetTraceType() const
	{
		return 0;
	}

	const CEntity* skip;
};

// I know this differs slightly from the one I used in the
// video... this one is more accurate :)
class CTrace
{
public:
	CVector start;
	CVector end;
	char pad[20];
	float fraction;
	int contents;
	unsigned short dispFlags;
	bool allSolid;
	bool startSolid;
	char pad1[4];
	struct Surface_t {
		const char* name;
		short surfaceProps;
		unsigned short flags;
	} surface;
	int hitgroup;
	char pad2[4];
	CEntity* entity;

	// some unknown extra data in something in or under ctrace has extra data
	// so adding a random extra garbage pad here for now to prevent memory overwriting. Very ugly
	char testPad[4];
};

class IEngineTraceClient
{
public:
	constexpr void TraceRay(
		const CRay& ray,
		const std::uintptr_t mask,
		const CTraceFilter& filter,
		CTrace& trace) noexcept
	{
		return memory::Call<void, const CRay&, const std::uintptr_t, const CTraceFilter&, CTrace&>(
			this,
			5,
			ray,
			mask,
			filter,
			trace
			);
	}
};