#pragma once

#include "tracy/Tracy.hpp" // IWYU pragma: export

// Helper for making sure frames end in all possible control flow path
class FrameMarker
{
	const char *m_name;
	bool m_started = false;
public:
	FrameMarker(const char *name) : m_name(name) {}
	~FrameMarker() { end(); }

	FrameMarker(const FrameMarker &) = delete;
	FrameMarker &operator=(const FrameMarker &) = delete;

	FrameMarker(FrameMarker &&other) noexcept :
		m_name(other.m_name), m_started(other.m_started)
	{
		other.m_started = false;
	}
	FrameMarker &operator=(FrameMarker &&other) noexcept
	{
		if (&other != this) {
			end();
			m_name = other.m_name;
			m_started = other.m_started;
			other.m_started = false;
		}
		return *this;
	}
	FrameMarker &&started() &&
	{
		if (!m_started) {
			FrameMarkStart(m_name);
			m_started = true;
		}
		return std::move(*this);
	}
	void start()
	{
		// no move happens, because we drop the reference
		(void)std::move(*this).started();
	}
	void end()
	{
		if (m_started) {
			m_started = false;
			FrameMarkEnd(m_name);
		}
	}
};
