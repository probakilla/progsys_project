#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "error.h"

#ifdef PADAWAN

void map_new (unsigned width, unsigned height)
{
    map_allocate (width, height);

    for (int x = 0; x < width; x++)
	map_set (x, height - 1, 0); // Ground

    for (int y = 0; y < height - 1; y++) {
	map_set (0, y, 1); // Wall
	map_set (width - 1, y, 1); // Wall
    }

    // Ajout de pieces.
    for (int x = 4; x < 12; x += 2)
	map_set (x, height - 5, 6);

    // Ajout de fleurs fixes.
    map_set (3, height - 2, 4);
    map_set (10, height - 2, 5);

    map_object_begin (7);
    // Texture pour le sol
    map_object_add ("images/ground.png", 1, MAP_OBJECT_SOLID);
    // Mur
    map_object_add ("images/wall.png", 1, MAP_OBJECT_SOLID);
    // Gazon
    map_object_add ("images/grass.png", 1, MAP_OBJECT_SEMI_SOLID);
    // Marbre
    map_object_add ("images/marble.png", 1, MAP_OBJECT_SOLID | MAP_OBJECT_DESTRUCTIBLE);
    // Fleurs
    map_object_add ("images/flower.png", 1, MAP_OBJECT_AIR);
    map_object_add ("images/flower2.png", 1, MAP_OBJECT_AIR);
    // Pieces
    map_object_add ("images/coin.png", 20, MAP_OBJECT_AIR | MAP_OBJECT_COLLECTIBLE); 
  
    map_object_end ();

}

void map_save (char *filename)
{
    int fd = fopen ("save/saved.map", O_CREAT | O_WRONLY | O_TRUNC, 0640);

    // Ecrit respectivement dans le fichier de sauvegarde les dimensions de la carte et le nombre d'objets diférents.
    write_data (fd, map_width ());
    write_data (fd, map_height ());
    write_data (fd, map_objects ());

    for (int y = 0; y < map_height () - 1; ++y)
	for (int x = 0; x < map_widht (); ++x)
	{
	    write_data (fd, x);
	    write_data (fd, y);
	    write_data (fd, map_get(x, y));
	}
    
    fprintf (stderr, "Sorry: Map save is not yet implemented\n");
}

void map_load (char *filename)
{
    // TODO
    exit_with_error ("Map load is not yet implemented\n");
}

// Ecrit dans un fichier fd la donnée data
void write_data (int fd, int data)
{
    fwrite (&data, sizeof(data), 1, fd);
}

#endif