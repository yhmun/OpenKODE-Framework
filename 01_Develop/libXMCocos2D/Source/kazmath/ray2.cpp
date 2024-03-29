/* -----------------------------------------------------------------------------------
 *
 *      File            ray2.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2008      Luke Benstead. All rights reserved.    
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Redistribution and use in source and binary forms, with or without modification,
 *      are permitted provided that the following conditions are met:
 *      
 *       * Redistributions of source code must retain the above copyright notice,
 *         this list of conditions and the following disclaimer.
 *       * Redistributions in binary form must reproduce the above copyright notice,
 *         this list of conditions and the following disclaimer in the documentation
 *         and/or other materials provided with the distribution.
 *      
 *       THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *       ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *       WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *       DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *       ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *       (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *       LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *       ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *       (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *       SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "kazmath/ray2.h"

void kmRay2Fill(kmRay2* ray, kmScalar px, kmScalar py, kmScalar vx, kmScalar vy) {
    ray->start.x = px;
    ray->start.y = py;
    ray->dir.x = vx;
    ray->dir.y = vy;
}

kmBool kmRay2IntersectLineSegment(const kmRay2* ray, const kmVec2* p1, const kmVec2* p2, kmVec2* intersection) {

    float x1 = ray->start.x;
    float y1 = ray->start.y;
    float x2 = ray->start.x + ray->dir.x;
    float y2 = ray->start.y + ray->dir.y;
    float x3 = p1->x;
    float y3 = p1->y;
    float x4 = p2->x;
    float y4 = p2->y;

    float denom = (y4 -y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
    float ua, x, y;
    //If denom is zero, the lines are parallel
    if(denom > -kmEpsilon && denom < kmEpsilon) {
        return KM_FALSE;
    }

    ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom;
//    float ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom;

    x = x1 + ua * (x2 - x1);
    y = y1 + ua * (y2 - y1);

    if(x < kmMin(p1->x, p2->x) - kmEpsilon ||
       x > kmMax(p1->x, p2->x) + kmEpsilon ||
       y < kmMin(p1->y, p2->y) - kmEpsilon ||
       y > kmMax(p1->y, p2->y) + kmEpsilon) {
        //Outside of line
        //printf("Outside of line, %f %f (%f %f)(%f, %f)\n", x, y, p1->x, p1->y, p2->x, p2->y);
        return KM_FALSE;
    }

    if(x < kmMin(x1, x2) - kmEpsilon ||
       x > kmMax(x1, x2) + kmEpsilon ||
       y < kmMin(y1, y2) - kmEpsilon ||
       y > kmMax(y1, y2) + kmEpsilon) {
        //printf("Outside of ray, %f %f (%f %f)(%f, %f)\n", x, y, x1, y1, x2, y2);
        return KM_FALSE;
    }

    intersection->x = x;
    intersection->y = y;

    return KM_TRUE;


/*
    kmScalar A1, B1, C1;
    kmScalar A2, B2, C2;

    A1 = ray->dir.y;
    B1 = ray->dir.x;
    C1 = A1 * ray->start.x + B1 * ray->start.y;

    A2 = p2->y - p1->y;
    B2 = p2->x - p1->x;
    C2 = A2 * p1->x + B2 * p1->y;

    double det = (A1 * B2) - (A2 * B1);
    if(det == 0) {
        printf("Parallel\n");
        return KM_FALSE;
    }

    double x = (B2*C1 - B1*C2) / det;
    double y = (A1*C2 - A2*C1) / det;

    if(x < min(p1->x, p2->x) - kmEpsilon ||
       x > max(p1->x, p2->x) + kmEpsilon ||
       y < min(p1->y, p2->y) - kmEpsilon ||
       y > max(p1->y, p2->y) + kmEpsilon) {
        //Outside of line
        printf("Outside of line, %f %f (%f %f)(%f, %f)\n", x, y, p1->x, p1->y, p2->x, p2->y);
        return KM_FALSE;
    }

    kmScalar x1 = ray->start.x;
    kmScalar x2 = ray->start.x + ray->dir.x;

    kmScalar y1 = ray->start.y;
    kmScalar y2 = ray->start.y + ray->dir.y;

    if(x < min(x1, x2) - kmEpsilon ||
       x > max(x1, x2) + kmEpsilon ||
       y < min(y1, y2) - kmEpsilon ||
       y > max(y1, y2) + kmEpsilon) {
        printf("Outside of ray, %f %f (%f %f)(%f, %f)\n", x, y, x1, y1, x2, y2);
        return KM_FALSE;
    }

    intersection->x = x;
    intersection->y = y;

    return KM_TRUE;*/
}

void calculate_line_normal(kmVec2 p1, kmVec2 p2, kmVec2* normal_out) {
    kmVec2 tmp;
    kmVec2Subtract(&tmp, &p2, &p1); //Get direction vector

    normal_out->x = -tmp.y;
    normal_out->y = tmp.x;
    kmVec2Normalize(normal_out, normal_out);

    //TODO: should check that the normal is pointing out of the triangle
}

kmBool kmRay2IntersectTriangle(const kmRay2* ray, const kmVec2* p1, const kmVec2* p2, const kmVec2* p3, kmVec2* intersection, kmVec2* normal_out) {
    kmVec2 intersect;
    kmVec2 final_intersect = { 0.f, 0.f }, normal = { 0.f, 0.f }; // Silencing LLVM SA.
    kmScalar distance = 10000.0f;
    kmBool intersected = KM_FALSE;

    if(kmRay2IntersectLineSegment(ray, p1, p2, &intersect)) {
        kmVec2 tmp;
        kmScalar this_distance;

        intersected = KM_TRUE;
        this_distance = kmVec2Length(kmVec2Subtract(&tmp, &intersect, &ray->start));
        if(this_distance < distance) {
            final_intersect.x = intersect.x;
            final_intersect.y = intersect.y;
            distance = this_distance;

            calculate_line_normal(*p1, *p2, &normal);
        }
    }

    if(kmRay2IntersectLineSegment(ray, p2, p3, &intersect)) {
        kmVec2 tmp;
        kmScalar this_distance;
        intersected = KM_TRUE;

        this_distance = kmVec2Length(kmVec2Subtract(&tmp, &intersect, &ray->start));
        if(this_distance < distance) {
            final_intersect.x = intersect.x;
            final_intersect.y = intersect.y;
            distance = this_distance;

            calculate_line_normal(*p2, *p3, &normal);
        }
    }

    if(kmRay2IntersectLineSegment(ray, p3, p1, &intersect)) {
        kmVec2 tmp;
        kmScalar this_distance;
        intersected = KM_TRUE;

        this_distance = kmVec2Length(kmVec2Subtract(&tmp, &intersect, &ray->start));
        if(this_distance < distance) {
            final_intersect.x = intersect.x;
            final_intersect.y = intersect.y;            

            calculate_line_normal(*p3, *p1, &normal);
        }
    }

    if(intersected) {
        intersection->x = final_intersect.x;
        intersection->y = final_intersect.y;
        if(normal_out) {
            normal_out->x = normal.x;
            normal_out->y = normal.y;
        }
    }

    return intersected;
}

kmBool kmRay2IntersectCircle(const kmRay2* ray, const kmVec2 centre, const kmScalar radius, kmVec2* intersection) {
    kdAssert(0 && "Not implemented");
    return 0;
}
