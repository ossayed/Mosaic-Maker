
// File:        tileUtil.cpp
// Author:      Cinda
// Date:        2020-10-31
// Description: file handling functions that prepare data for the tiling algorithm


#include "tileUtil.h"

/**
 * Function tile:
 * @param PNG & target: an image to use as base for the mosaic. it's pixels will be
 *                      be replaced by thumbnail images whose average color is close
 *                      to the pixel.
 * @param rgbtree & ss: a kd-tree of RGBAPixels, used as a query structure for
 *                      nearest neighbor search. 
 * @param map<RGBAPixel, string> & photos: a map that takes a color key and returns the
 *                      filename of an image whose average color is that key.
 *
 * returns: a PNG whose dimensions are TILESIZE times that of the target. Each
 * pixel in the target is used as a query to ss.findNearestNeighbor, and the response
 * is used as a key in photos. 
 */

PNG tiler::tile(PNG & target, const rgbtree & ss, map<RGBAPixel,string> & photos){

/* your code here! */
    
	int height = target.height();
	int width = target.width();
    PNG output = PNG(width*TILESIZE,height*TILESIZE);

	for(int x = 0; x<width;x++){
		for(int y = 0; y<height;y++){

			RGBAPixel closecolour = ss.findNearestNeighbor(*(target.getPixel(x,y)));
            PNG tile; 
            string photo = photos[closecolour];
            tile.readFromFile(photo);

            for(int x1 = 0;x1< tile.width();x1++){ // x1<TILESIZE
                for(int y1=0; y1<tile.height();y1++){
                    *output.getPixel(x1 + TILESIZE*x,y1 + TILESIZE*y) = *tile.getPixel(x1,y1);



                }
            }
			
		}
	}
    
    return output;


}

/* buildMap: function for building the map of <key, value> pairs, where the key is an
 * RGBAPixel representing the average color over an image, and the value is 
 * a string representing the path/filename.png of the TILESIZExTILESIZE image
 * whose average color is the key.
 * 
 * We've provided a bit of the C++ code that allows you to iterate over the files
 * in a directory. It is up to you to figure out what it means, and how to use it.
 * 
 * @param path is the subdirectory in which the tiles can be found. In our examples
 * this is imlib.
 *
*/
map<RGBAPixel, string> tiler::buildMap(string path) {

    map < RGBAPixel, string> thumbs;
    for (const auto & entry : fs::directory_iterator(path)) {
    	//int mapinsert = 0; // not necessary
        PNG curr; curr.readFromFile(entry.path());
        int avrgr = 0;
    	int avrgg = 0;
    	int avrgb = 0;
    	int height = curr.height();
    	int width = curr.width();

    	for(int x = 0; x < width;x++){

    		for(int y =0; y<height;y++){
    			avrgr = curr.getPixel(x,y)->r + avrgr;
    			avrgg = curr.getPixel(x,y)->g + avrgg;
    			avrgb = curr.getPixel(x,y)->b + avrgb;

    		}
    	}
    	avrgr = avrgr/(curr.width()*curr.height()); // get all other averages
    	avrgg = avrgg/(curr.width()*curr.height());
    	avrgb= avrgb/(curr.width()*curr.height());
    	RGBAPixel avrgpix = RGBAPixel(avrgr,avrgg,avrgb);
    	thumbs[avrgpix] = entry.path();


    	//humbs.insert({avrgpix,entry.path()});
    	//mapinsert++;



    /* your code here */


    }
    return thumbs;
}


