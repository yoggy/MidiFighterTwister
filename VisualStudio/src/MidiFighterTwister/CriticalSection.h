#pragma once

class CriticalSection;

class ScopedCriticalSection {
public:
	ScopedCriticalSection(CriticalSection &cs);
	~ScopedCriticalSection();

private:
	CriticalSection *cs_;
};

class CriticalSection
{
public:
	CriticalSection();
	~CriticalSection();

	void lock();
	void unlock();

private:
	// noncopyable
	CriticalSection(const CriticalSection &obj) {}
	CriticalSection& operator=(const CriticalSection &obj) { return *this; }

	CRITICAL_SECTION cs_;
};

