/* Copyright (c) 2012-2016 Stanislaw Halik
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 */
#include "ftnoir_filter_accela.h"
#include "compat/math.hpp"
#include "api/plugin-api.hpp"

#include <algorithm>
#include <QDebug>
#include <QMutexLocker>

#include "compat/math-imports.hpp"
#include "compat/time.hpp"

accela::accela() : first_run(true)
{
    s.make_splines(spline_rot, spline_pos);
}

template<int N = 3, typename F>
cc_noinline
static void do_deltas(const double* deltas, double* output, F&& fun)
{
    double norm[N];

    const double dist = progn(
        double ret = 0;
        for (unsigned k = 0; k < N; k++)
            ret += deltas[k]*deltas[k];
        return sqrt(ret);
    );

    const double value = double(fun(dist));

    for (unsigned k = 0; k < N; k++)
    {
        const double c = dist > 1e-6 ? clamp((fabs(deltas[k]) / dist), 0., 1.) : 0;
        norm[k] = c;
    }

    progn(
        double n = 0;
        for (unsigned k = 0; k < N; k++)
            n += norm[k];

        if (n > 1e-6)
        {
            const double ret = 1./n;
            for (unsigned k = 0; k < N; k++)
                norm[k] *= ret;
        }
        else
            for (unsigned k = 0; k < N; k++)
                norm[k] = 0;
    );

    for (unsigned k = 0; k < N; k++)
    {
        const double d = norm[k] * value;
        output[k] = signum(deltas[k]) * d;
    }
}

void accela::filter(const double* input, double *output)
{
    if (unlikely(first_run))
    {
        first_run = false;

        for (int i = 0; i < 6; i++)
        {
            const double f = input[i];
            output[i] = f;
            last_output[i] = f;
        }

        t.start();
#if defined DEBUG_ACCELA
        debug_max = 0;
        debug_timer.start();
#endif

        return;
    }

    const double rot_thres = s.rot_smoothing.to<double>();
    const double pos_thres = s.pos_smoothing.to<double>();

    const double dt = t.elapsed_seconds();
    t.start();

    const double rot_dz = s.rot_deadzone.to<double>();
    const double pos_dz = s.pos_deadzone.to<double>();

    // rot

    for (unsigned i = 3; i < 6; i++)
    {
        double d = input[i] - last_output[i];

        if (fabs(d) > rot_dz)
            d -= copysign(rot_dz, d);
        else
            d = 0;

        deltas[i] = d / rot_thres;
    }

    do_deltas(&deltas[Yaw], &output[Yaw], [this](double x) {
        return spline_rot.get_value_no_save(x);
    });

    // pos

    for (unsigned i = 0; i < 3; i++)
    {
        double d = input[i] - last_output[i];
        if (fabs(d) > pos_dz)
            d -= copysign(pos_dz, d);
        else
            d = 0;

        deltas[i] = d / pos_thres;
    }

    do_deltas(&deltas[TX], &output[TX], [this](double x) {
        return spline_pos.get_value_no_save(x);
    });

    // end

    for (unsigned k = 0; k < 6; k++)
    {
        output[k] *= dt;
        output[k] += last_output[k];

        last_output[k] = output[k];
    }
}

void settings_accela::make_splines(spline& rot, spline& pos)
{
    rot = spline();
    pos = spline();

    for (const auto& val : rot_gains)
        rot.add_point(QPointF(val.x, val.y));

    for (const auto& val : pos_gains)
        pos.add_point(QPointF(val.x, val.y));
}

OPENTRACK_DECLARE_FILTER(accela, dialog_accela, accelaDll)

