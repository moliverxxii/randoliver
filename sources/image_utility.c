/*
 * imgUtil.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include "main.h"

const image_t SYSTEM_SCREEN =
{
    1280,
    720,
    NULL
};


image_t*
init_image(int width, int height)
{

    colour_t* image_data_p = malloc(height * width * sizeof(colour_t));

    image_t* image = (image_t*) malloc(sizeof(image_t));
    image->width   = width;
    image->height  = height;
    image->image   = (row_t*) malloc(height * sizeof(row_t));

    uint32_t y;
	row_t row_p = image_data_p;
    for(y = 0; y < height; ++y)
    {
        image->image[y] = row_p;
        row_p += width;
    }
    return image;
}

void
set_image(image_t* image)
{
    memset(*image->image, 0, image->width * image->height * sizeof(colour_t) );
}

void
random_image(image_t* image_p)
{
    int x,y;
    for(x=0; x<image_p->width; ++x)
    {
        for(y=0; y<image_p->height; ++y)
        {
            *(colour_struct_t*) image_p->image[y][x] = get_random_colour();
        }
    }
}

void
process_1_image(colour_unary_operator operator, image_t* image_p, void* parameters_p)
{
    int x,y;
    for(x=0; x<image_p->width; ++x)
    {
        for(y=0; y<image_p->height; ++y)
        {
            *(colour_struct_t*) image_p->image[y][x] =  (*operator)( *(colour_struct_t*) image_p->image[y][x], parameters_p);
        }
    }
}

void
process_2_images(colour_binary_operator operator,
        image_t* image_1_p,
        const image_t* image_2_p,
        void* parameters_p)
{
    int x,y;
    for(x=0; x<image_1_p->width; ++x)
    {
        for(y=0; y<image_1_p->height; ++y)
        {
            *(colour_struct_t*) image_1_p->image[y][x] =
                    (*operator)( *(colour_struct_t*) image_1_p->image[y][x],
                                 *(colour_struct_t*) image_2_p->image[y][x],
                                 parameters_p);
        }
    }
}

void
process_3_images(colour_ternary_operator operator,
        image_t* image_1_p,
        const image_t* image_2_p,
        const image_t* image_3_p,
        void* parameters_p)
{
    int x,y;
    for(x=0; x<image_1_p->width; ++x)
    {
        for(y=0; y<image_1_p->height; ++y)
        {
            *(colour_struct_t*) image_1_p->image[y][x] =
                    (*operator)( *(colour_struct_t*) image_1_p->image[y][x],
                                 *(colour_struct_t*) image_2_p->image[y][x],
                                 *(colour_struct_t*) image_3_p->image[y][x],
                                 parameters_p);
        }
    }
}

void add_images(image_t* image_1_p, const image_t* image_2_p)
{
    process_2_images(&add_colours, image_1_p, image_2_p, NULL);
}

void
disp_image(image_t* image)
{
    int x;
    int y;
    for(x = 0; x < image->width; ++x)
    {
        for(y = 0; y < image->height; ++y)
        {
            printf("Point[%5d,%5d] = (%x,%x,%x)\n", x + 1, y + 1,
                    image->image[y][x][0], image->image[y][x][1],
                    image->image[y][x][2]);
        }
    }
}

void
draw_rect(colour_t color, int botLeftX, int botLeftY, int topRightX,
          int topRightY, image_t* image)
{
    int x;
    int y;
    int k;
    for(x = botLeftX; x <= topRightX; ++x)
    {
        for(y = botLeftY; y <= topRightY; ++y)
        {
            for(k = 0; k < 3; ++k)
            {
                image->image[y][x][k] = color[k];
            }
        }
    }
}


point_renderer public_point_renderer = &or_point;

void
draw_point(const point_t point, image_t* image_p)
{
    int x;
    int y;
    x = point.vector.x;
    y = point.vector.y;
    if(is_in_image(x, y, image_p))
    {
        *(colour_struct_t*) image_p->image[y][x] = *(colour_struct_t*) &point.colour;
    }
}

void
or_point(const point_t point, image_t* image_p)
{
    int x;
    int y;
    x = point.vector.x;
    y = point.vector.y;
    if(!is_in_image(x, y, image_p))
    {
        return;
    }
    uint32_t buffer = *(uint32_t*) image_p->image[y][x];
    buffer ^= *(uint32_t*) &point.colour;
    *(colour_struct_t*) image_p->image[y][x] = *(colour_struct_t*) &buffer;

}


void
xor_point(const point_t point, image_t* image_p)
{
    int x;
    int y;
    x = point.vector.x;
    y = point.vector.y;
    if(!is_in_image(x, y, image_p))
    {
        return;
    }
    uint32_t buffer = *(uint32_t*) image_p->image[y][x];
    buffer ^= *(uint32_t*) &point.colour;
    *(colour_struct_t*) image_p->image[y][x] = *(colour_struct_t*) &buffer;

}

void
average_point(const point_t point, image_t* image_p)
{
    int x;
    int y;
    x = point.vector.x;
    y = point.vector.y;
    if(!is_in_image(x, y, image_p))
    {
    	return;
    }
    for(int colour=0; colour<COLOUR_COUNT; ++colour)
    {
    	image_p->image[y][x][colour] = (uint8_t) ((
    												(uint32_t) image_p->image[y][x][colour]
												  + (uint32_t) ((uint8_t*) &point.colour)[colour])/2);
    }
}

int
is_in_image(int x, int y, const image_t* image_p)
{
    return (x >= 0) && (x < image_p->width) && (y >= 0) && (y < image_p->height);
}

void
draw_figure(image_t* image, figure_t* figure)
{
    unsigned int i;
    for(i = 0; i < figure->amount; ++i)
    {
    	(*public_point_renderer)(figure->sequence[i],image);
    }
}


void
free_image(image_t* image)
{
    free(*(image->image));
    free(image->image);
    free(image);
}

void
brownien1(image_t* image, int iterations, int spread, int x0, int y0)
{
    colour_t pixels[1] =
    {
        {0, 0xFF, 0}
    };
    int y = y0;
    int x = x0;
    int color = 0;
    int delta = 0;
    int i;
    iterations = 1000000;
    //Début du procesus.
    printf("Début du processus\n");
    for(i = 0; i < iterations; ++i)
    {
        for(color = 0; color < 3; ++color)
        {

            image->image[y][x][color] = pixels[0][color];
            if(color == 1)
            {
                delta = rand() % 3 - 1;
                pixels[0][color] = saturator(pixels[0][color] + delta,
                SAT_MIN,
                SAT_MAX);
            }
        }
        delta = rand() % (2 * spread + 1) - spread;
        y += delta;
        y = modulo(y, image->height);

        delta = rand() % (2 * spread + 1) - spread;
        x += delta;
        x = modulo(x, image->width);
    }
    printf("Processus terminé\n");
}

void
barres1(image_t* image, int spread)
{
    printf("Début du processus\n");

    int i = 0;
    int y;
    int x;
    int x_prime;
    int bool = 1;

    for(y = 0; y < image->height; ++y)
    {
        x = 2;
        x_prime = x;
        bool = 1;
        while(bool)
        {

            for(i = 0; i < 3; ++i)
            {
                *(colour_struct_t*) image->image[y][x_prime] = WHITE;
            }
            x += spread;
            x_prime = x + (rand() % 5 - 2);
            bool = x_prime < image->width;
        }
    }
    printf("Processus terminé\n");

}

void
barres2(image_t* image, int spread)
{
    printf("Début du processus\n");

    int i = 0;
    int y;
    int x;
    uint32_t x_prime[3];
    int bool;

    for(y = 0; y < image->height; ++y)
    {
        x = 2;
        for(i = 0; i < 3; ++i)
            x_prime[i] = x;
        bool = 1;
        while(bool)
        {

            for(i = 0; i < 3; ++i)
            {
                *(colour_struct_t*) image->image[y][x_prime[i]] = WHITE;
            }
            x += spread;
            for(i = 0; i < 3; ++i)
                x_prime[i] = x + (rand() % 5 - 2);
            bool = (x_prime[0] < image->width) & (x_prime[1] < image->width)
                    & (x_prime[2] < image->width);
        }
    }
    printf("Processus terminé\n");
}


void
init_camera(camera_t* camera_p,
		    float origin_x, float origin_y, float origin_z,
			float destin_x, float destin_y, float destin_z,
			float distance)
{
	camera_t camera_loc =
	{
		{
			origin_x,
			origin_y,
			origin_z
		},
		{
			destin_x,
			destin_y,
			destin_z
		},
		distance
	};
    (*camera_p) = camera_loc;
}


void
render_figure(image_t* image_p, figure_t figure, camera_t camera)
{
	printf("Render start.\n");

    float distance = camera.distance;
    vector_t o = camera.origin;
    vector_t f = camera.direction;
    vector_t of;
    subtract_vectors(&of, f, o);

    float norme_of = norm_vector(of);
    vector_t u;
    scale_vector(&u, of, 1/norme_of);

    vector_t v;
    v.x = -u.y / sqrt(pow(u.x, 2) + pow(u.y, 2));
    v.y =  u.x / sqrt(pow(u.x, 2) + pow(u.y, 2));
    v.z = 0;

    vector_t w;
    w.x = -u.z * u.x / sqrt(pow(u.x, 2) + pow(u.y, 2));
    w.y =  u.z * u.y / sqrt(pow(u.x, 2) + pow(u.y, 2));
    w.z = sqrt(pow(u.x, 2) + pow(u.y, 2));

    for(int i = 0; i < figure.amount; ++i)
    {
        vector_t p = figure.sequence[i].vector;

        vector_t op;
        subtract_vectors(&op, p, o);

        float op_u_scalaire = scalar_vector(op, u);
        float op_v_scalaire = scalar_vector(op, v);
        float op_w_scalaire = scalar_vector(op, w);

        int x_image = SYSTEM_SCREEN.width/2
                    + distance * op_v_scalaire / op_u_scalaire;
        int y_image = SYSTEM_SCREEN.height/2
                    + distance * op_w_scalaire / op_u_scalaire;

        if(op_u_scalaire <= 0)
        {
            continue;
        }
        point_t render_point =
        {
            {
                x_image,
                y_image,
                0,
            },
            figure.sequence[i].colour
        };
        (*public_point_renderer)(render_point, image_p);
    }
	printf("Render end.\n");

}
