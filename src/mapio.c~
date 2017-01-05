#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "error.h"

#define MAX_NAME_LENGTH 32

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
    filename = "maps/saved.map";
    FILE* fd = fopen (filename, "w+");

    // Ecrit respectivement dans le fichier de sauvegarde les dimensions de la carte et le nombre d'objets diférents.
    int width = map_width ();
    int height = map_height ();
    int nb_objects = map_objects ();
    int i, y, x, name_length, object, sprites, solidity, destructible, collectible, generator;
    
    fwrite (&width, sizeof (width), 1, fd);
    fwrite (&height, sizeof (height), 1, fd);
    fwrite (&nb_objects, sizeof (nb_objects), 1, fd);

    // Ecriture des objets
    for (i = 0; i < nb_objects; i ++)
    {
	object = i;
	fwrite (&object, sizeof (object), 1, fd);
	name_length = strlen (map_get_name (object));
	char *name = calloc (name_length , sizeof (char));
	strcpy (name, map_get_name (object));
	
	fwrite (&name_length, sizeof (name_length), 1, fd);
	fwrite (name, name_length * sizeof (*name), 1, fd);
	free (name);
	
	sprites = map_get_frames (object);
	solidity = map_get_solidity (object);
	destructible = map_is_destructible (object);
	collectible = map_is_collectible (object);
	generator = map_is_generator (object);
	
	fwrite (&sprites, sizeof (sprites), 1, fd);
	fwrite (&solidity, sizeof (solidity), 1, fd);	
	fwrite (&destructible, sizeof (destructible), 1, fd);
	fwrite (&collectible, sizeof (collectible), 1, fd);
	fwrite (&generator, sizeof (generator), 1, fd);
    }
    
    for (y = 0; y < height; ++y)
	for (x = 0; x < width; ++x)
	{
	    object = map_get (x, y);
	    fwrite (&x, sizeof (x), 1, fd);
	    fwrite (&y, sizeof (y), 1, fd);
	    fwrite (&object, sizeof(object), 1, fd);		 		
	}
    fclose (fd);
    printf ("You saved the map in:  %s. \n", filename);
}

void map_load (char *filename)
{
    FILE * fd = fopen (filename, "r+");

    int i, width, height, x, y, name_length, nb_objects, object, sprites, solidity, destructible, collectible, generator;

    // Charge les dimentions de la carte
    fread (&width, sizeof(width), 1, fd);
    fread (&height, sizeof(height), 1, fd);
    fread (&nb_objects, sizeof(nb_objects), 1, fd);
  
    map_allocate (width, height);

   
    map_object_begin (nb_objects);
    
    for (i = 0; i < nb_objects; ++i)
    {
	fread (&object, sizeof (object), 1, fd);
	fread (&name_length, sizeof (name_length), 1, fd);
	char * name = calloc (name_length, sizeof (char));

	fread (name, name_length * sizeof (*name), 1, fd);

	fread (&sprites, sizeof (sprites), 1, fd);
	fread (&solidity, sizeof (solidity), 1, fd);
	fread (&destructible, sizeof (destructible), 1, fd);
	fread (&collectible, sizeof (collectible), 1, fd);
	fread (&generator, sizeof (generator), 1, fd);
	if (!destructible && !collectible && !generator)
	{
	    map_object_add (name, sprites, solidity);
	}
	else if (destructible && !collectible && !generator)
	    map_object_add (name, sprites, solidity | MAP_OBJECT_DESTRUCTIBLE);
	else if (!destructible && collectible && !generator)
	    map_object_add (name, sprites, solidity | MAP_OBJECT_COLLECTIBLE);
	else if (!destructible && !collectible && generator)
	    map_object_add (name, sprites, solidity | MAP_OBJECT_GENERATOR);
	else if (destructible && collectible && !generator)
	    map_object_add (name, sprites, solidity | MAP_OBJECT_DESTRUCTIBLE | MAP_OBJECT_COLLECTIBLE);
	else if (!destructible && collectible && generator)
	    map_object_add (name, sprites, solidity | MAP_OBJECT_COLLECTIBLE | MAP_OBJECT_GENERATOR);
	else if (destructible && collectible && generator)
	    map_object_add (name, sprites, solidity | MAP_OBJECT_DESTRUCTIBLE | MAP_OBJECT_COLLECTIBLE | MAP_OBJECT_GENERATOR);

	free(name);
    }
    
    map_object_end ();

    for (i = 0; i < (height * width) - 1; ++i)
    {
	fread (&x, sizeof(x), 1, fd);
	fread (&y, sizeof(y), 1, fd);
	fread (&object, sizeof(object), 1, fd);
	if (object != -1)
	    map_set (x, y, object);
    }
    fclose (fd);

}

#endif
