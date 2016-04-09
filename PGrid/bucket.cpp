﻿#include "bucket.h"
#include <assert.h>
#include "secondary_index.h"


pair<Bucket*, unsigned> Bucket::Add(int id, float x, float y, int col, int row) {
	assert(current_ < kSize);
	sites_[current_].id = id;
	sites_[current_].x = x;
	sites_[current_].y = y;
	sites_[current_].tu = static_cast<long int>(time(nullptr));
	return make_pair(this, current_++);
}

tuple<int, Bucket*, unsigned, bool, bool> Bucket::Del(const int id) {
	auto result = make_tuple(-1, static_cast<Bucket *>(nullptr), 0, true, false);
	auto p_bucket = this;
	while (p_bucket) {
		auto p = find_if(p_bucket->sites_.begin(), p_bucket->sites_.begin() + p_bucket->current_,
		                 [id] (const Site& s) {
			                 return s.id == id;
		                 });
		if (p != p_bucket->sites_.begin() + p_bucket->current_) {
			if (s_mutex[this->sites_[current_ - 1].id].try_lock()) {
				lock_guard<mutex>{s_mutex[this->sites_[current_ - 1].id], adopt_lock};
				--current_;
				get<0>(result) = this->sites_[current_].id;
				get<1>(result) = p_bucket;
				get<2>(result) = p - p_bucket->sites_.begin();
				get<3>(result) = this->sites_[current_].tu > 0;
				get<4>(result) = true;

				*p = this->sites_[current_];
				goto end_while;
			}
			else return result;
		}
		else p_bucket = p_bucket->next_.get();
	}
end_while:
	return result;
}
