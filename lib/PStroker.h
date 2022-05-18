#ifndef PSTROKER_H
#define PSTROKER_H

#include <PShape.h>
#include <PMath.h>
#include <optional>


#define miterMinAngle 0.349066f
#define roundMinAngle 0.174533f

#define MITER   0
#define BEVEL   1

#define SQUARE  0
#define PROJECT 1

#define ROUND   2

/**
 * JOIN:
 *  - MITTER
 *  - BEVEL
 *  - ROUND
 * CAP:
 *  - SQUARE
 *  - PROJECT
 *  - ROUND
 */


struct LineSegment {
    LineSegment(const PVector &a, const PVector &b) : a(a), b(b) {}
    PVector a, b;

    LineSegment operator+(const PVector &toAdd) const {
        return {PVector::add(a, toAdd), PVector::add(b, toAdd)};
    }
    LineSegment operator-(const PVector &toRemove) const {
        return {PVector::sub(a, toRemove), PVector::sub(b, toRemove)};
    }
    PVector normal() const {
        auto dir = direction();
        return {-dir.y, dir.x};
    }
    PVector direction(bool normalized = true) const {
        auto vec = PVector::sub(b, a);
        return normalized ? vec.normalize() : vec;
    }

    static std::optional<PVector> intersection(const LineSegment &a, const LineSegment &b, bool infiniteLines) {
        // calculate un-normalized direction vectors
        auto r = a.direction(false);
        auto s = b.direction(false);

        auto originDist = PVector::sub(b.a, a.a);

        auto uNumerator = originDist.cross(r);
        auto denominator = r.cross(s);

        if (std::abs(denominator.z) < 0.0001f) {
            // The lines are parallel
            return std::nullopt;
        }

        // solve the intersection positions
        auto u = uNumerator.z / denominator.z;
        auto t = PVector::cross(originDist, s).z / denominator.z;

        if (!infiniteLines && (t < 0 || t > 1 || u < 0 || u > 1)) {
            // the intersection lies outside of the line segments
            return std::nullopt;
        }

        // calculate the intersection point
        // a.a + r * t;
        return PVector::add(a.a, PVector::mult(r, t));
    }
};

struct PolySegment {
    PolySegment(const LineSegment &center, float thickness) :
            center(center),
            edge1(center + PVector::mult(center.normal(), thickness)),
            edge2(center - PVector::mult(center.normal(), thickness)) {}
    LineSegment center, edge1, edge2;
};

PShape createTriangleFan(PVector connectTo, PVector origin,
                         PVector start, PVector end, bool clockwise) {

    PShape vertices;
        auto point1 = PVector::sub(start, origin);
        auto point2 = PVector::sub(end, origin);

        // calculate the angle between the two points
        auto angle1 = atan2(point1.y, point1.x);
        auto angle2 = atan2(point2.y, point2.x);

        // ensure the outer angle is calculated
        if (clockwise) {
            if (angle2 > angle1) {
                angle2 = angle2 - 2 * PI;
            }
        } else {
            if (angle1 > angle2) {
                angle1 = angle1 - 2 * PI;
            }
        }

        auto jointAngle = angle2 - angle1;

        // calculate the amount of triangles to use for the joint
        auto numTriangles = std::max(1, (int) std::floor(std::abs(jointAngle) / roundMinAngle));

        // calculate the angle of each triangle
        auto triAngle = jointAngle / numTriangles;

        PVector startPoint = start;
        PVector endPoint;
        for (int t = 0; t < numTriangles; t++) {
            if (t + 1 == numTriangles) {
                // it's the last triangle - ensure it perfectly
                // connects to the next line
                endPoint = end;
            } else {
                auto rot = (t + 1) * triAngle;

                // rotate the original point around the origin
                endPoint.x = std::cos(rot) * point1.x - std::sin(rot) * point1.y;
                endPoint.y = std::sin(rot) * point1.x + std::cos(rot) * point1.y;

                // re-add the rotation origin to the target point
                endPoint = PVector::add(endPoint, origin);
            }

            // emit the triangle
            vertices.push(startPoint);
            vertices.push(endPoint  );
            vertices.push(connectTo );

            startPoint = endPoint;
        }

        //std::cout << "Size -> "<< vertices.size() << "\n";
        return vertices;
    }


PShape createJoint( const PolySegment &segment1, const PolySegment &segment2,
                    int jointStyle, PVector &end1, PVector &end2,
                    PVector &nextStart1, PVector &nextStart2,
                    bool allowOverlap) {

    PShape vertices;
        // calculate the angle between the two line segments
        auto dir1 = segment1.center.direction();
        auto dir2 = segment2.center.direction();

        auto angle = PVector::angleBetween(dir1, dir2);

        // wrap the angle around the 180° mark if it exceeds 90°
        // for minimum angle detection
        auto wrappedAngle = angle;
        if (wrappedAngle > PI / 2) {
            wrappedAngle = PI - wrappedAngle;
        }

        if (jointStyle == MITER && wrappedAngle < miterMinAngle) {
            jointStyle =  BEVEL;
        }

        if (jointStyle == MITER) {
            // calculate each edge's intersection point
            // with the next segment's central line
            auto sec1 = LineSegment::intersection(segment1.edge1, segment2.edge1, true);
            auto sec2 = LineSegment::intersection(segment1.edge2, segment2.edge2, true);

            end1 = sec1 ? *sec1 : segment1.edge1.b;
            end2 = sec2 ? *sec2 : segment1.edge2.b;

            nextStart1 = end1;
            nextStart2 = end2;

        } else {
            // joint style is either BEVEL or ROUND

            // find out which are the inner edges for this joint
            auto x1 = dir1.x;
            auto x2 = dir2.x;
            auto y1 = dir1.y;
            auto y2 = dir2.y;

            auto clockwise = x1 * y2 - x2 * y1 < 0;

            const LineSegment *inner1, *inner2, *outer1, *outer2;

            // as the normal vector is rotated counter-clockwise,
            // the first edge lies to the left
            // from the central line's perspective,
            // and the second one to the right.
            if (clockwise) {
                outer1 = &segment1.edge1;
                outer2 = &segment2.edge1;
                inner1 = &segment1.edge2;
                inner2 = &segment2.edge2;
            } else {
                outer1 = &segment1.edge2;
                outer2 = &segment2.edge2;
                inner1 = &segment1.edge1;
                inner2 = &segment2.edge1;
            }

            // calculate the intersection point of the inner edges
            auto innerSecOpt = LineSegment::intersection(*inner1, *inner2, allowOverlap);

            auto innerSec = innerSecOpt
                            ? *innerSecOpt
                            // for parallel lines, simply connect them directly
                            : inner1->b;

            // if there's no inner intersection, flip
            // the next start position for near-180° turns
            PVector innerStart;
            if (innerSecOpt) {
                innerStart = innerSec;
            } else if (angle > PI / 2) {
                innerStart = outer1->b;
            } else {
                innerStart = inner1->b;
            }

            if (clockwise) {
                end1 = outer1->b;
                end2 = innerSec;

                nextStart1 = outer2->a;
                nextStart2 = innerStart;

            } else {
                end1 = innerSec;
                end2 = outer1->b;

                nextStart1 = innerStart;
                nextStart2 = outer2->a;
            }

            // connect the intersection points according to the joint style

            if (jointStyle == BEVEL) {
                // simply connect the intersection points
                vertices.push(outer1->b);
                vertices.push(outer2->a);
                vertices.push(innerSec);

            } else if (jointStyle == ROUND) {
                // draw a circle between the ends of the outer edges,
                // centered at the actual point
                // with half the line thickness as the radius
                vertices.add(createTriangleFan(innerSec, segment1.center.b, outer1->b, outer2->a, clockwise));
            }
        }

        return vertices;
    }

PShape strokify(PShape contour, float w, int cap, int join,bool allowOverlap = false){
    PShape data(TRIANGLES);
    data.setColor(contour.getColor());
    bool loop = contour.loop();
    // operate on half the thickness to make our lives easier
    w /= 2;

    // create poly segments from the points
    std::vector<PolySegment> segments;
    for (size_t i = 0; i + 1 < contour.size(); i++) {
        auto point1 = contour.at(i);
        auto point2 = contour.at(i + 1);

        auto p1 = createVector(point1.x,
                               point1.y,
                               point1.z);
        auto p2 = createVector(point2.x,
                               point2.y,
                               point2.z);

        // to avoid division-by-zero errors,
        // only create a line segment for non-identical points
        if (!PVector::equal(p1, p2)) {
            segments.emplace_back(LineSegment(p1, p2), w);
        }
    }

    if (segments.empty()) {
        // handle the case of insufficient input points
        return contour;
    }

    PVector nextStart1;
    PVector nextStart2;
    PVector start1;
    PVector start2;
    PVector end1;
    PVector end2;

    // calculate the path's global start and end points
    auto &firstSegment = segments[0];
    auto &lastSegment = segments[segments.size() - 1];

    auto pathStart1 = firstSegment.edge1.a;
    auto pathStart2 = firstSegment.edge2.a;
    auto pathEnd1 = lastSegment.edge1.b;
    auto pathEnd2 = lastSegment.edge2.b;

    if (loop) {
            data.add(createJoint(lastSegment, firstSegment, join,
                        pathEnd1, pathEnd2, pathStart1, pathStart2, allowOverlap));
    }else {
        if (cap == SQUARE) {
            pathStart1 = PVector::sub(pathStart1, PVector::mult(firstSegment.edge1.direction(), w));
            pathStart2 = PVector::sub(pathStart2, PVector::mult(firstSegment.edge2.direction(), w));
            pathEnd1   = PVector::add(pathEnd1,   PVector::mult(lastSegment.edge1.direction(),  w));
            pathEnd2   = PVector::add(pathEnd2,   PVector::mult(lastSegment.edge2.direction(),  w));

        } else if (cap == ROUND) {
            data.add(createTriangleFan(firstSegment.center.a, firstSegment.center.a,
                              firstSegment.edge1.a, firstSegment.edge2.a, false));
            data.add(createTriangleFan(lastSegment.center.b, lastSegment.center.b,
                              lastSegment.edge1.b, lastSegment.edge2.b, true));
        }
    }

    // generate mesh data for path segments
    for (size_t i = 0; i < segments.size(); i++) {
        auto &segment = segments[i];

        // calculate start
        if (i == 0) {
            // this is the first segment
            start1 = pathStart1;
            start2 = pathStart2;
        }

        if (i + 1 == segments.size()) {
            // this is the last segment
            end1 = pathEnd1;
            end2 = pathEnd2;

        } else {
          data.add(createJoint(segment, segments[i + 1], join,
                      end1, end2, nextStart1, nextStart2, allowOverlap));
        }

        // emit vertices
       data.push(start1);
       data.push(start2);
       data.push(end1  );

       data.push(end1  );
       data.push(start2);
       data.push(end2  );

        start1 = nextStart1;
        start2 = nextStart2;
    }

    return data;
}

#endif // PSTROKER_H
