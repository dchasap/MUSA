//
// Copyright (C) 2009-2012
// Barcelona Supercomputing Center - Centro Nacional de Supercomputacion
// Universitat Politecnica de Catalunya
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//

#include "core/engine/types.h"
#include "core/utils/RangeSet.h"
#include "core/utils/RangeMap.h"

#include "gtest/gtest.h"

using namespace sim;

typedef std::map<engine::addr_t, engine::addr_t> range_set;
typedef std::map<engine::addr_t, std::pair<engine::addr_t, size_t>> range_map;

TEST(RangeSetTest, Contains)
{
    range_set m;
    m[64] = 256;

    utils::RangeSet r({64, 256});
    ASSERT_EQ(r, m);

    ASSERT_TRUE( r.contains(64) );
    ASSERT_TRUE( r.contains(127) );
    ASSERT_TRUE( r.contains(128) );
    ASSERT_TRUE( r.contains(156) );
    ASSERT_TRUE( r.contains(255) );

    ASSERT_FALSE( r.contains(0) );
    ASSERT_FALSE( r.contains(63) );
    ASSERT_FALSE( r.contains(256) );
    ASSERT_FALSE( r.contains(1375) );

    utils::RangeSet s({0, -1});

    ASSERT_TRUE( s.contains(0) );
    ASSERT_TRUE( s.contains(15641) );
    ASSERT_TRUE( s.contains(-2) );
    ASSERT_FALSE( s.contains(-1) );
}


TEST(RangeSetTest, RoundingInserts)
{
    range_set m;
    m[64] = 128;

    utils::RangeSet r;

    r.insert({64, 128});
    ASSERT_EQ(r, m);

    r.insert({67, 128});
    ASSERT_EQ(r, m);

    r.insert({192, 256});

    r.insert({128, 192});
    m[64] = 256;
    ASSERT_EQ(r, m);

    m.erase(64);
    m[0] = 256;
    r.insert({0, 64});

    ASSERT_EQ(r, m);
}


TEST(RangeSetTest, Merge)
{
    range_set m;
    m[64] = 8*64;

    // overlapping merge: end
    utils::RangeSet n({64, 3*64}), o({2*64, 8*64});

    n.merge(o);
    ASSERT_EQ(n, m);

    // overlapping merge: full overlap
    utils::RangeSet p({2*64, 3*64}), q({64, 8*64});

    p.merge(q);
    ASSERT_EQ(p, m);

    // overlapping merge: start, included
    utils::RangeSet r({2*64, 8*64}), s({64, 3*64}), t({6*64, 7*64});

    s.merge(t);
    r.merge(s);
    ASSERT_EQ(r, m);

    // contiguous merge
    utils::RangeSet u({3*64, 8*64}), v({64, 2*64}), w({2*64, 3*64});

    u.merge(v);
    u.merge(w);
    ASSERT_EQ(u, m);

    // non-overlapping merge
    m[10*64] = 12*64;
    utils::RangeSet x({10*64, 12*64});

    u.merge(x);
    ASSERT_EQ(u, m);
}


TEST(RangeSetTest, Position)
{
    range_set m;
    m[64] = 255;
    m[512] = 767;

    utils::RangeSet r({64, 255}), n({512, 767});
    r.merge(n);

    ASSERT_EQ(r, m);

    ASSERT_EQ(   0, r.position(64) );
    ASSERT_EQ(  64, r.position(128) );

    ASSERT_EQ(  -1, r.position(0) );
    ASSERT_EQ(  -1, r.position(63) );
    ASSERT_EQ(  -1, r.position(256) );
    ASSERT_EQ(  -1, r.position(1375) );

    ASSERT_EQ(  -1, r.position(500) );
    ASSERT_EQ(  -1, r.position(15641) );
    ASSERT_EQ( 256, r.position(576) );
    ASSERT_EQ( 192, r.position(512) );
}


TEST(RangeSetTest, Maps)
{
    utils::RangeMap map;
    utils::RangeSet ext({8, 32}), mid({16, 24});

    ASSERT_EQ(-1, map.get_range_index( 4));
    ASSERT_EQ(-1, map.get_range_index(12));
    ASSERT_EQ(-1, map.get_range_index(20));
    ASSERT_EQ(-1, map.get_range_index(28));
    ASSERT_EQ(-1, map.get_range_index(36));

    map.merge(mid, 0);

    ASSERT_EQ(-1, map.get_range_index( 4));
    ASSERT_EQ(-1, map.get_range_index(12));
    ASSERT_EQ( 0, map.get_range_index(16));
    ASSERT_EQ( 0, map.get_range_index(20));
    ASSERT_EQ(-1, map.get_range_index(24));
    ASSERT_EQ(-1, map.get_range_index(28));
    ASSERT_EQ(-1, map.get_range_index(36));

    map.merge(ext, 1);

    ASSERT_EQ(-1, map.get_range_index( 4));
    ASSERT_EQ( 1, map.get_range_index(12));
    ASSERT_EQ( 0, map.get_range_index(16));
    ASSERT_EQ( 0, map.get_range_index(20));
    ASSERT_EQ( 1, map.get_range_index(24));
    ASSERT_EQ( 1, map.get_range_index(28));
    ASSERT_EQ(-1, map.get_range_index(36));

    map.merge(utils::RangeSet({56, 64}), 1);
    map.merge(utils::RangeSet({40, 48}), 1);
    map.merge(utils::RangeSet({0, 72}), 1);

    ASSERT_EQ( 1, map.get_range_index(36));
    ASSERT_EQ( 1, map.get_range_index(40));
    ASSERT_EQ( 1, map.get_range_index(48));
    ASSERT_EQ( 1, map.get_range_index(52));
    ASSERT_EQ( 1, map.get_range_index(56));
    ASSERT_EQ( 1, map.get_range_index(64));
    ASSERT_EQ(-1, map.get_range_index(72));
}


TEST(RangeSetTest, MapMerge)
{
    range_map m;
    m[64] = {8*64, 1};

    // overlapping merge: end
    utils::RangeMap n;
    n.merge(utils::RangeSet({64, 3*64}), 1);
    n.merge(utils::RangeSet({2*64, 8*64}), 1);

    ASSERT_EQ(n, m);

    // overlapping merge: full overlap
    utils::RangeMap p;
    p.merge(utils::RangeSet({2*64, 3*64}), 1);
    p.merge(utils::RangeSet({64, 8*64}), 1);

    ASSERT_EQ(p, m);

    // overlapping merge: start, included
    utils::RangeMap r;
    r.merge(utils::RangeSet({2*64, 8*64}), 1);

    utils::RangeSet s({64, 3*64});
    s.insert({6*64, 7*64});

    r.merge(s, 1);
    ASSERT_EQ(r, m);

    // contiguous merge
    utils::RangeMap u;
    u.merge(utils::RangeSet({3*64, 8*64}), 1);
    u.merge(utils::RangeSet({64, 2*64}), 1);
    u.merge(utils::RangeSet({2*64, 3*64}), 1);

    ASSERT_EQ(u, m);

    // non-overlapping merge
    m[10*64] = {12*64, 1};
    u.merge(utils::RangeSet({10*64, 12*64}), 1);

    ASSERT_EQ(u, m);
}
