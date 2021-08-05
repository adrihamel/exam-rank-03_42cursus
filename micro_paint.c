/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguerrer </var/mail/aguerrer>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/05 20:16:37 by aguerrer          #+#    #+#             */
/*   Updated: 2021/08/05 20:18:29 by aguerrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int width;
int height;
char bg;

char type;
float x;
float y;
float w;
float h;
char color;

char *draw;

void output()
{  
    int i = -1;
    
    while (++i < height)
    {
        write(1, draw + i * width, width);
        write(1, "\n", 1);
    }
    free(draw);
}

int check(float x1, float y1)
{
    if (x1 < x || x1 > x + w || y1 < y || y1 > y + h)
		return (0);
	if (x1 - x < 1 || 1 > x + w - x1 || y1 - y < 1 || 1 > y + h - y1)
		return (2);
	return (1);
}

void drawing()
{
    int x1 = -1;
    int y1;
    int hit;

    while (++x1 < width)
    {
        y1 = -1;
        while (++y1 < height)
        {
            hit = check((float)x1, (float)y1);
            if (hit == 2 || (hit == 1 && type == 'R'))
                draw[x1 + y1 * width] = color;
        }
    }
}

int parseo(FILE *file)
{
    int i = 0;
    int ret = 0;

    if (fscanf(file, "%d %d %c\n", &width, &height, &bg) != 3)
        return(1);
    if (width > 300 || width < 1 || height > 300 || height < 1)
        return(1);
    if ((draw = malloc(width * height)) == NULL)
        return(1);
    while (i < width * height)
        draw[i++] = bg;
    while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &type, &x, &y, &w, &h, &color)) ==  6)
    {
        if (w <= 0 || h <= 0 || (type != 'r' && type != 'R'))
            return(1);
        drawing();
    }
    if (ret != -1)
        return(1);
    output();
    return(0);
}

int main (int argc, char **argv)
{
    FILE *file;

    if (argc != 2)
    {
        write(1, "Error: argument\n", 16);
        return(1);
    }
    if (!(file = fopen(argv[1], "r")) || parseo(file))
    {
        write(1, "Error: Operation file corrupted\n", 32);
        return(1);
    }
    fclose(file);
    return(0);
}
