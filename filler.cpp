/**
 * @file filler.cpp
 * Implementation of functions in the filler namespace.
 *
 */

/**
 * Performs a multi-point flood fill using breadth first search.
 *
 * @param config      FillerConfig struct to setup the fill
 * @return animation  object illustrating progression of flood fill algorithm
 */
animation filler::fillBFS(FillerConfig &config)
{
    return fill<Queue>(config);
}

/**
 * Performs a multi-point flood fill using depth first search.
 *
 * @param config      FillerConfig struct to setup the fill
 * @return animation  object illustrating progression of flood fill algorithm
 */
animation filler::fillDFS(FillerConfig &config)
{
    return fill<Stack>(config);
}

/**
 * Run a multi-point flood fill on an image
 *
 * @param  config     FillerConfig struct with data for flood fill of image
 * @return animation  object illustrating progression of flood fill algorithm
 */
template <template <class T> class OrderingStructure> animation filler::fill(FillerConfig &config)
{
    OrderingStructure<point> os;
    animation result;

    PNG image = config.img;
    int imgW = image.width();
    int imgH = image.height();
    
    int frameCounter = 1;

    /* vector<vector<bool>> tracker;
    for (int i = 0; i < imgW; i++) {
        vector<bool> row;
        for (int j = 0; j < imgH; j++) {
            row.push_back(false);
        }
        tracker.push_back(row);
    }  */

    bool tracker[480][205] = {{false}};


    for (int c = 0; c < (int) config.centres.size(); c++) {
        centre ctr = config.centres[c];
        ColorPicker *picker = config.pickers[c];
        point ctrP = point(ctr);
        
        os.add(ctrP);
        HSLAPixel *pixel = image.getPixel(ctrP.x, ctrP.y);
        *pixel = picker->operator()(ctrP);
        frameCounter++;
        updateFrame(frameCounter, config.frameFreq, result, config.img);
        tracker[ctrP.x][ctrP.y] = true; 

        while (!os.isEmpty()) {
            point curr = os.remove();

            if ((int) curr.y - 1 < imgH && (int) curr.y - 1 >= 0 && tracker[curr.x][curr.y - 1] == false) {  // check up
                HSLAPixel *nextPixel =  image.getPixel(curr.x, curr.y - 1);

                if (ctr.color.dist(*nextPixel) <= config.tolerance) {
                    point next = point(curr.x, curr.y - 1, ctr);

                    *nextPixel = picker->operator()(next);
                    os.add(next);

                    frameCounter++;
                    if (frameCounter == config.frameFreq) {
                        result.addFrame(image);
                        frameCounter = 1;
                    }

                    tracker[next.x][next.y] = true;
                }
            } 
            if ((int) curr.x + 1 < imgW && (int) curr.x + 1 >= 0 && tracker[curr.x + 1][curr.y]  == false) {  // check right
                HSLAPixel *nextPixel = image.getPixel(curr.x + 1, curr.y);

                if (ctr.color.dist(*nextPixel) <= config.tolerance) {
                    point next = point(curr.x + 1, curr.y, ctr);

                    *nextPixel = picker->operator()(next);
                    os.add(next);

                    frameCounter++;
                    if (frameCounter == config.frameFreq) {
                        result.addFrame(image);
                        frameCounter = 1;
                    }
                    tracker[next.x][next.y] = true;
                }
            } 
            
            if ((int) curr.y + 1 < imgH && (int) curr.y + 1 >= 0 && tracker[curr.x][curr.y + 1]  == false) {  // check down
                HSLAPixel *nextPixel = image.getPixel(curr.x, curr.y  + 1);

                if (ctr.color.dist(*nextPixel) <= config.tolerance) {
                    point next = point(curr.x, curr.y + 1, ctr);

                    *nextPixel = picker->operator()(next);
                    os.add(next);

                    frameCounter++;
                    if (frameCounter == config.frameFreq) {
                        result.addFrame(image);
                        frameCounter = 1;
                    }

                    tracker[next.x][next.y] = true;
                } 
            } 
            
            if ((int) curr.x - 1 < imgW && (int) curr.x - 1 >= 0 && tracker[curr.x - 1][curr.y]  == false) {  // check left
                HSLAPixel *nextPixel = image.getPixel(curr.x - 1, curr.y);

                if (ctr.color.dist(*nextPixel) <= config.tolerance) {
                    point next = point(curr.x - 1, curr.y, ctr);

                    *nextPixel = picker->operator()(next);
                    os.add(next);
                    
                    frameCounter++;
                    if (frameCounter == config.frameFreq) {
                        result.addFrame(image);
                        frameCounter = 1;
                    }
                    
                    tracker[next.x][next.y] = true;
                }
            } 
        }
    }

    /* for (int a = 0; a < (int) image.width(); a++) { 
        tracker[a].clear();
    } */

    //delete[] tracker;

    result.addFrame(image);
    return result;

     /**
     *
     * This is the basic description of a flood-fill algorithm: Every fill
     * algorithm requires an ordering structure, which is passed to this
     * function via its template parameter. For a breadth-first-search
     * fill, that structure is a Queue, for a depth-first-search, that
     * structure is a Stack. To begin the algorithm, you simply place the
     * given point in the ordering structure, marking it processed
     * (the way you mark it is a design decision you'll make yourself).
     * We have a choice to either change the color, if appropriate, when we
     * add the point to the OS, or when we take it off. In our test cases,
     * we have assumed that you will change the color when a point is added
     * to the structure.
     * Until the structure is empty, you do the following:
     *
     * 1.     Remove a point from the ordering structure, and then...
     *
     *        1.    add its unprocessed neighbors whose color values are
     *              within (or equal to) tolerance distance from the center,
     *              to the ordering structure, and
     *              mark them as processed.
     *        2.    if it is an appropriate frame, send the current PNG to the
     *              animation (as described below).
     *
     * 2.     When implementing your breadth-first-search and
     *        depth-first-search fills, you will need to explore neighboring
     *        pixels in some order.
     *
     *        While the order in which you examine neighbors does not matter
     *        for a proper fill, you must use the same order as we do for
     *        your animations to come out like ours! The order you should put
     *        neighboring pixels **ONTO** the queue or stack is as follows:
     *        ** UP(-y), RIGHT(+x), DOWN(+y), LEFT(-x). IMPORTANT NOTE: *UP*
     *        here means towards the top of the image, so since an image has
     *        smaller y coordinates at the top, this is in the *negative y*
     *        direction. Similarly, *DOWN* means in the *positive y*
     *        direction.** To reiterate, when you are exploring (filling out)
     *        from a given pixel, you must first try to fill the pixel UP
     *        from it, then the one to its RIGHT, then DOWN from it and
     *        finally to its LEFT. If you do them in a different order, your fill may
     *        still work correctly, but your animations will be different
     *        from the grading scripts!
     *
     *        Because we are doing a multi-point flood fill you will repeat the flood
     *        fill from several locations stored in the `centres` variable of FillerConfig
     *        struct. Initial fill points should be visited in the order they stored in the
     *        `centres` vector. If a pixel lies in the fill region of multiple start points,
     *        it should only be updated by the first fill. To repeat, pixels should only
     *        be modified the first time they are visited by the filler algorithm.
     *
     * 3.     For every k pixels filled, **starting at the kth pixel**, you
     *        must add a frame to the animation, where k = frameFreq.
     *
     *        For example, if frameFreq is 4, then after the 4th pixel has
     *        been filled you should add a frame to the animation, then again
     *        after the 8th pixel, etc.  You must only add frames for the
     *        number of pixels that have been filled, not the number that
     *        have been checked. So if frameFreq is set to 1, a pixel should
     *        be filled every frame.
     *
     * 4.     Finally, as you leave the function, send one last frame to the
     *        animation. This frame will be the final result of the fill, and
     *        it will be the one we test against.
     *
     */
}

void filler::updateFrame(int frameCounter, int frameFreq, animation& result, PNG img) {
    if (frameCounter % frameFreq == 0) {
        result.addFrame(img);
    }
}
