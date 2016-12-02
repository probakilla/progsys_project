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
    FILE* fd = fopen (filename, "w+");

    // Ecrit respectivement dans le fichier de sauvegarde les dimensions de la carte et le nombre d'objets diférents.
    int width = map_width ();
    int height = map_height ();
    int nb_objects = map_objects ();
    int cpt = 0;
    int t[nb_objects];
    int i, y, x, object_section, object, sprites, solidity, destructible, collectible, generator;
    char name [MAX_NAME_LENGTH];
    
    fwrite (&width, sizeof (width), 1, fd);
    fwrite (&height, sizeof (height), 1, fd);
    fwrite (&nb_objects, sizeof (nb_objects), 1, fd);
    //ject_section = (((width * height) * 3) * 4) + 32; 

    // Initialise toutes les valeurs du tableau à -2, une valeur représentant l'absence d'objet.
    for (i = 0; i < nb_objects; ++i)
	t[i] = -2;
    
    for (y = 0; y < height - 1; ++y)
	for (x = 0; x < width; ++x)
	{
	    object = map_get (x, y);
	    fwrite (&x, sizeof (x), 1, fd);
	    fwrite (&y, sizeof (y), 1, fd);
	    fwrite (&object, sizeof(object), 1, fd);
	    
	    if (cpt == 0)
	    {
		t [0] = object;
		++cpt;
	    }
	    for (i = 0; i < cpt; ++i)
	    {
		if (t[i] == object)
		    break;  
		else if (t[i] == -2)
		{
		    t[i] = object;
		    ++cpt;
		    printf("%d - ", object);
		    printf("\n");
		    break;
		}	      	    
	    }			 		
	}
    
    for (i = 0; i < map_objects(); ++i)
    {
	object = t[i];
	strcpy (map_get_name(object), name);
	//name = map_get_name(object);
	sprites = map_get_frames(object);
	solidity = map_get_solidity (object);
	destructible = map_is_destructible (object);
	collectible = map_is_collectible (object);
	generator = map_is_generator (object);
	
	fwrite (&name, sizeof(name), 1, fd);
	fwrite (&sprites, sizeof (sprites), 1, fd);
        fwrite (&solidity, sizeof(solidity), 1, fd);
	fwrite (&destructible, sizeof(destructible), 1, fd);
	fwrite (&collectible, sizeof (collectible), 1, fd);
        fwrite (&generator, sizeof(generator), 1, fd);	
    }
    printf ("You saved the map in:  %s.", filename);
}

void map_load (char *filename)
{/*
    FILE * fd = fopen (filename, "r+");

    int i, width, height, x, y, nb_objects, object, sprites, solidity, destructible, collectible, generator;
    object_section = (((width * height) * 3) * 4) + 32;
    char name [MAX_NAME_LENGTH];

    // Charge les dimentions de la carte
    fread (&width, sizeof(width), 1, fd);
    fread (&height, sizeof(height), 1, fd);
    fread (&nb_objects, sizeof(nb_objects), 1, fd);
    
    map_allocate (width, height);

    for (i = 0; i < (height * width) - 1; ++i)
    {
	fread (&x, sizeof(x), 1, fd);
	fread (&y, sizeof(y), 1, fd);
	fread (&object, sizeof(object), 1, fd);
	if (object != -1)
	    map_set (x, y, object);
    }
    
    map_object_begin (nb_objects);
    
    for (i = 0; i < nb_objects; ++i)
    {
	map_object_add ("images/ground.png", 1, MAP_OBJECT_SOLID);
    }
    map_object_end ();

    
    exit_with_error ("Map load is not yet implemented\n");*/
}

#endif
