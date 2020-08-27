#pragma once

#include "gl_loader.hpp"
#include "animation_util.hpp"

#include <ozz/base/log.h>
#include <ozz/base/maths/simd_math.h>
#include <ozz/base/maths/soa_transform.h>
#include <ozz/base/maths/vec_float.h>
#include <ozz/base/maths/box.h>

#include <ozz/animation/runtime/animation.h>
#include <ozz/animation/runtime/local_to_model_job.h>
#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/animation/runtime/skeleton.h>

#include <vector>

// class PlaybackSampleApplication : public ozz::sample::Application
namespace Rxt
{
struct skeletal_animation
{
    // Updates current animation time and skeleton pose.
    bool update(float dt)
    {
        // Updates current animation time.
        // _controller.Update(_animation, dt);
        using ozz::make_span;

        // Samples optimized animation at t = _animation_time.
        ozz::animation::SamplingJob sampling_job;
        sampling_job.animation = &_animation;
        sampling_job.cache = &_cache;
        // sampling_job.ratio = _controller.time_ratio();
        sampling_job.output = make_span(_locals);
        if (!sampling_job.Run()) {
            return false;
        }

        // Converts from local space to model space matrices.
        ozz::animation::LocalToModelJob ltm_job;
        ltm_job.skeleton = &_skeleton;
        ltm_job.input = make_span(_locals);
        ltm_job.output = make_span(_models);
        if (!ltm_job.Run()) {
            return false;
        }

        return true;
    }

    virtual void scene_bounds(ozz::math::Box* bound) const
    {
        ozz::sample::ComputePostureBounds(ozz::make_span(_models), bound);
    }

    // Runtime
    ozz::animation::Skeleton _skeleton;
    ozz::animation::Animation _animation;

    ozz::animation::SamplingCache _cache;

    // Buffer of local transforms as sampled from _animation.
    std::vector<ozz::math::SoaTransform> _locals;
    // Buffer of model space matrices.
    std::vector<ozz::math::Float4x4> _models;

    gl::asset_loader loader;

    bool load(gl::asset_loader& loader)
    {
        // Reading skeleton.
        if (!ozz::sample::LoadSkeleton(loader.read_file("skeleton.ozz").c_str(), &_skeleton)) {
            return false;
        }

        // Reading animation.
        if (!ozz::sample::LoadAnimation(loader.read_file("animation.ozz").c_str(), &_animation)) {
            return false;
        }

        // Skeleton and animation needs to match.
        if (_skeleton.num_joints() != _animation.num_tracks()) {
            return false;
        }

        // Allocates runtime buffers.
        const int num_soa_joints = _skeleton.num_soa_joints();
        _locals.resize(num_soa_joints);
        const int num_joints = _skeleton.num_joints();
        _models.resize(num_joints);

        // Allocates a cache that matches animation requirements.
        _cache.Resize(num_joints);

        return true;
    }};

}
