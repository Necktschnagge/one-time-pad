#pragma once

#include "global_data.h"
#include "action_error.h"

#include <string>

class action {
public:

	inline static const global_data* data{ nullptr };
private:
	std::size_t id;
	inline static std::size_t new_free_id{ 0 };
protected:

	void assert_match() {
		if (!match()) throw action_error("This action does not match the input");
	}
	virtual void act() = 0;

public:
	action() : id(new_free_id++) {}

	virtual bool match() const = 0;

	inline void run() { assert_match(); act(); }

	virtual std::string description() const = 0;

	inline bool operator==(const action& another) const { return another.id == id; }
};
