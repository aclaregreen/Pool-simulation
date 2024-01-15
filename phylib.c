#include "phylib.h"

phylib_object *phylib_new_still_ball( unsigned char number, phylib_coord *pos ) {
    phylib_object *new_obj = (phylib_object *)malloc(sizeof(phylib_object));

    if (new_obj == NULL){
        return NULL;
    }

    new_obj->type = PHYLIB_STILL_BALL;
    new_obj->obj.still_ball.number = number;
    new_obj->obj.still_ball.pos = *pos;
    return new_obj;
}

phylib_object *phylib_new_rolling_ball( unsigned char number, phylib_coord *pos, phylib_coord *vel, phylib_coord *acc ){
    phylib_object *new_obj = (phylib_object *)malloc(sizeof(phylib_object));

    if (new_obj == NULL){
        return NULL;
    }
    new_obj->type = PHYLIB_ROLLING_BALL;
    new_obj->obj.rolling_ball.number = number;
    new_obj->obj.rolling_ball.pos = *pos;
    new_obj->obj.rolling_ball.vel = *vel;
    new_obj->obj.rolling_ball.acc = *acc;
    return new_obj;
}

phylib_object *phylib_new_hole( phylib_coord *pos ){
    phylib_object *new_obj = (phylib_object *)malloc(sizeof(phylib_object));

    if (new_obj == NULL){
        return NULL;
    }
    new_obj->type = PHYLIB_HOLE;
    new_obj->obj.hole.pos = *pos;
    return new_obj;
}

phylib_object *phylib_new_hcushion( double y ){
    phylib_object *new_obj = (phylib_object *)malloc(sizeof(phylib_object));

    if (new_obj == NULL){
        return NULL;
    }
    new_obj->type = PHYLIB_HCUSHION;
    new_obj->obj.hcushion.y = y;
    return new_obj;
}

phylib_object *phylib_new_vcushion( double x ){
    phylib_object *new_obj = (phylib_object *)malloc(sizeof(phylib_object));

    if (new_obj == NULL){
        return NULL;
    }
    new_obj->type = PHYLIB_VCUSHION;
    new_obj->obj.vcushion.x = x;
    return new_obj;
}

phylib_table *phylib_new_table( void ){
    phylib_table *new_table = (phylib_table *)malloc(sizeof(phylib_table));
    if (new_table == NULL){
        return NULL;
    }
    for (int i = 0; i < PHYLIB_MAX_OBJECTS; i++){
        new_table->object[i] = NULL;
    }
    new_table->time = 0.0;
    new_table->object[0] = phylib_new_hcushion(0.0);
    new_table->object[1] = phylib_new_hcushion(PHYLIB_TABLE_LENGTH);
    new_table->object[2] = phylib_new_vcushion(0.0);
    new_table->object[3] = phylib_new_vcushion(PHYLIB_TABLE_WIDTH);
    new_table->object[4] = phylib_new_hole(&(phylib_coord){0.0,0.0});
    new_table->object[5] = phylib_new_hole(&(phylib_coord){0.0, PHYLIB_TABLE_WIDTH});
    new_table->object[6] = phylib_new_hole(&(phylib_coord){PHYLIB_TABLE_LENGTH/2, 0.0});
    new_table->object[7] = phylib_new_hole(&(phylib_coord){PHYLIB_TABLE_LENGTH/2, PHYLIB_TABLE_WIDTH/2});
    new_table->object[8] = phylib_new_hole(&(phylib_coord){PHYLIB_TABLE_LENGTH, 0.0});
    new_table->object[9] = phylib_new_hole(&(phylib_coord){PHYLIB_TABLE_LENGTH, PHYLIB_TABLE_WIDTH});

    return new_table;
}
//part 2 utility functions
void phylib_copy_object( phylib_object **dest, phylib_object **src ){
    //phylib_obj new_obj = malloc(sizeof(phylib_object));
}

phylib_table *phylib_copy_table( phylib_table *table ){

}
void phylib_add_object( phylib_table *table, phylib_object *object ){

}
void phylib_free_table( phylib_table *table ){

}
phylib_coord phylib_sub( phylib_coord c1, phylib_coord c2 ){
    phylib_coord sub;
    sub.x = c1.x - c2.x;
    sub.y = c1.y - c2.y;
    return sub;
}
double phylib_length( phylib_coord c ){
    double length = sqrt(c.x * c.x + c.y * c.y);
    return length;
}
double phylib_dot_product( phylib_coord a, phylib_coord b ){
    double dot_product = 0;
    dot_product += (a.x * b.x);
    dot_product += (a.y * b.y);
    return dot_product;
}
double phylib_distance( phylib_object *obj1, phylib_object *obj2 ){

}