/*
 * preset_list.c
 *
 *  Created on: 5 juil. 2026
 *      Author: moliver
 */
void preset_add_middle_point();
void preset_add_corners();
void preset_add_lists();
void preset_add_vectors();
void preset_add_plane();
void preset_add_brown();
void preset_add_bitarray();
void preset_add_test_pattern();
void preset_add_palette();
void preset_add_squares();
void preset_add_solid_file();
void preset_add_figure();
void preset_add_sphere();

void
preset_list_init()
{
    preset_add_middle_point();
    preset_add_corners();
    preset_add_lists();
    preset_add_vectors();
    preset_add_plane();
    preset_add_brown();
    preset_add_bitarray();
    preset_add_test_pattern();
    preset_add_palette();
    preset_add_squares(); //TODO incrementation du numero de fichier.
    preset_add_solid_file();
    preset_add_figure();
    preset_add_sphere();
}
