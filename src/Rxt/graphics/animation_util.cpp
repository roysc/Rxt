#include "animation_util.hpp"


#include <ozz/base/maths/box.h>
#include <ozz/base/maths/simd_math.h>
#include <ozz/base/maths/simd_quaternion.h>
#include <ozz/base/maths/soa_transform.h>

#include <ozz/animation/runtime/animation.h>
#include <ozz/animation/runtime/local_to_model_job.h>
#include <ozz/animation/runtime/skeleton.h>
#include <ozz/animation/runtime/track.h>

namespace ozz {
namespace sample {

PlaybackController::PlaybackController()
    : time_ratio_(0.f),
      previous_time_ratio_(0.f),
      playback_speed_(1.f),
      play_(true),
      loop_(true) {}

void PlaybackController::Update(const animation::Animation& _animation,
                                float _dt) {
  float new_time = time_ratio_;

  if (play_) {
    new_time = time_ratio_ + _dt * playback_speed_ / _animation.duration();
  }

  // Must be called even if time doesn't change, in order to update previous
  // frame time ratio. Uses set_time_ratio function in order to update
  // previous_time_ an wrap time value in the unit interval (depending on loop
  // mode).
  set_time_ratio(new_time);
}

void PlaybackController::set_time_ratio(float _ratio) {
  previous_time_ratio_ = time_ratio_;
  if (loop_) {
    // Wraps in the unit interval [0:1], even for negative values (the reason
    // for using floorf).
    time_ratio_ = _ratio - floorf(_ratio);
  } else {
    // Clamps in the unit interval [0:1].
    time_ratio_ = math::Clamp(0.f, _ratio, 1.f);
  }
}

// Gets animation current time.
float PlaybackController::time_ratio() const { return time_ratio_; }

// Gets animation time of last update.
float PlaybackController::previous_time_ratio() const {
  return previous_time_ratio_;
}

void PlaybackController::Reset() {
  previous_time_ratio_ = time_ratio_ = 0.f;
  playback_speed_ = 1.f;
  play_ = true;
}

// Uses LocalToModelJob to compute skeleton model space posture, then forwards
// to ComputePostureBounds
void ComputeSkeletonBounds(const animation::Skeleton& _skeleton,
                           math::Box* _bound) {
  using ozz::math::Float4x4;

  assert(_bound);

  // Set a default box.
  *_bound = ozz::math::Box();

  const int num_joints = _skeleton.num_joints();
  if (!num_joints) {
    return;
  }

  // Allocate matrix array, out of memory is handled by the LocalToModelJob.
  ozz::vector<ozz::math::Float4x4> models(num_joints);

  // Compute model space bind pose.
  ozz::animation::LocalToModelJob job;
  job.input = _skeleton.joint_bind_poses();
  job.output = make_span(models);
  job.skeleton = &_skeleton;
  if (job.Run()) {
    // Forwards to posture function.
    ComputePostureBounds(job.output, _bound);
  }
}

// Loop through matrices and collect min and max bounds.
void ComputePostureBounds(ozz::span<const ozz::math::Float4x4> _matrices,
                          math::Box* _bound) {
  assert(_bound);

  // Set a default box.
  *_bound = ozz::math::Box();

  if (_matrices.empty()) {
    return;
  }

  // Loops through matrices and stores min/max.
  // Matrices array cannot be empty, it was checked at the beginning of the
  // function.
  const ozz::math::Float4x4* current = _matrices.begin();
  math::SimdFloat4 min = current->cols[3];
  math::SimdFloat4 max = current->cols[3];
  ++current;
  while (current < _matrices.end()) {
    min = math::Min(min, current->cols[3]);
    max = math::Max(max, current->cols[3]);
    ++current;
  }

  // Stores in math::Box structure.
  math::Store3PtrU(min, &_bound->min.x);
  math::Store3PtrU(max, &_bound->max.x);

  return;
}


bool LoadSkeleton(const char* _filename, ozz::animation::Skeleton* _skeleton) {
  assert(_filename && _skeleton);
  ozz::log::Out() << "Loading skeleton archive " << _filename << ".\n";
  ozz::io::File file(_filename, "rb");
  if (!file.opened()) {
      ozz::log::Err() << "Failed to open skeleton file " << _filename << ".\n";
    return false;
  }
  ozz::io::IArchive archive(&file);
  if (!archive.TestTag<ozz::animation::Skeleton>()) {
    ozz::log::Err() << "Failed to load skeleton instance from file "
                    << _filename << "." << std::endl;
    return false;
  }

  // Once the tag is validated, reading cannot fail.
  archive >> *_skeleton;

  return true;
}

bool LoadAnimation(const char* _filename,
                   ozz::animation::Animation* _animation) {
  assert(_filename && _animation);
  ozz::log::Out() << "Loading animation archive: " << _filename << "."
                  << std::endl;
  ozz::io::File file(_filename, "rb");
  if (!file.opened()) {
    ozz::log::Err() << "Failed to open animation file " << _filename << "."
                    << std::endl;
    return false;
  }
  ozz::io::IArchive archive(&file);
  if (!archive.TestTag<ozz::animation::Animation>()) {
    ozz::log::Err() << "Failed to load animation instance from file "
                    << _filename << "." << std::endl;
    return false;
  }

  // Once the tag is validated, reading cannot fail.
  archive >> *_animation;

  return true;
}
}
}
