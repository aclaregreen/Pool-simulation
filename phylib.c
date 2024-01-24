#include <stdio.h>
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
    new_table->time = PHYLIB_SIM_RATE;
    new_table->object[0] = phylib_new_hcushion(0.0);
    new_table->object[1] = phylib_new_hcushion(PHYLIB_TABLE_LENGTH);
    new_table->object[2] = phylib_new_vcushion(0.0);
    new_table->object[3] = phylib_new_vcushion(PHYLIB_TABLE_WIDTH);
    new_table->object[4] = phylib_new_hole(&(phylib_coord){0.0,0.0});
    new_table->object[5] = phylib_new_hole(&(phylib_coord){0.0, PHYLIB_TABLE_WIDTH});
    new_table->object[6] = phylib_new_hole(&(phylib_coord){PHYLIB_TABLE_LENGTH/2, 0.0});
    new_table->object[7] = phylib_new_hole(&(phylib_coord){PHYLIB_TABLE_LENGTH/2, PHYLIB_TABLE_WIDTH});
    new_table->object[8] = phylib_new_hole(&(phylib_coord){PHYLIB_TABLE_LENGTH, 0.0});
    new_table->object[9] = phylib_new_hole(&(phylib_coord){PHYLIB_TABLE_LENGTH, PHYLIB_TABLE_WIDTH});

    return new_table;

}
//part 2 utility functions
void phylib_copy_object( phylib_object **dest, phylib_object **src ){
    if (*src == NULL){
        *dest = NULL;
        return;
    }
    *dest = malloc(sizeof(phylib_object));
    if (*dest == NULL){
        return;
    }
    memcpy(*dest, *src, sizeof(phylib_object));
} 

phylib_table *phylib_copy_table( phylib_table *table ){
    phylib_table *copy_table = malloc(sizeof(phylib_table));
    if (copy_table == NULL){
        return NULL;
    }
    copy_table->time = table->time;
    for (int i = 0; i < PHYLIB_MAX_OBJECTS; i++){
        phylib_copy_object(&(copy_table->object[i]), &(table->object[i]));
    }
    return copy_table;
}
void phylib_add_object( phylib_table *table, phylib_object *object ){
    for (int i = 0; i < PHYLIB_MAX_OBJECTS; i++){
        if (table->object[i] == NULL){
            table->object[i] = object;
            return;
        }
    }
}
void phylib_free_table( phylib_table *table ){
    if (table == NULL) return;
    for (int i = 0; i < PHYLIB_MAX_OBJECTS; i++){
        free(table->object[i]);
    }
    free(table);
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
    if (obj1 == NULL || obj2 == NULL){
        return -1;
    }
    double distance;
    if (obj1->type != PHYLIB_ROLLING_BALL){
        return -1;
    }
    phylib_coord obj1_pos = obj1->obj.rolling_ball.pos;
    if (obj2->type == PHYLIB_STILL_BALL || obj2->type == PHYLIB_ROLLING_BALL){
        phylib_coord obj2_pos = (obj2->type == PHYLIB_STILL_BALL) ? obj2->obj.still_ball.pos : obj2->obj.rolling_ball.pos;
        distance = phylib_length(phylib_sub(obj1_pos, obj2_pos)) - PHYLIB_BALL_DIAMETER;
    } else if (obj2->type == PHYLIB_HOLE){
        phylib_coord obj2_pos = obj2->obj.hole.pos;
        distance = phylib_length(phylib_sub(obj1_pos, obj2_pos)) - PHYLIB_HOLE_RADIUS;
    } else if (obj2->type == PHYLIB_HCUSHION){
        double obj2_pos = obj2->obj.hcushion.y;
        distance = fabs(obj2_pos - obj1_pos.y - PHYLIB_BALL_RADIUS);
    } else if (obj2->type == PHYLIB_VCUSHION){
        double obj2_pos = obj2->obj.vcushion.x;
        distance = fabs(obj2_pos - obj1_pos.x - PHYLIB_BALL_RADIUS);
    } else {
        return -1;
    }
    return distance;
}
//part 3
void phylib_roll( phylib_object *new, phylib_object *old, double time ){
    if (new->type != PHYLIB_ROLLING_BALL || old->type != PHYLIB_ROLLING_BALL){
        return;
    }
    new->obj.rolling_ball.pos.x = old->obj.rolling_ball.pos.x + (old->obj.rolling_ball.vel.x * time) + (0.5 * old->obj.rolling_ball.acc.x * (time * time));
    new->obj.rolling_ball.pos.y = old->obj.rolling_ball.pos.y + (old->obj.rolling_ball.vel.y * time) + (0.5 * old->obj.rolling_ball.acc.y * (time * time));
    new->obj.rolling_ball.vel.x = old->obj.rolling_ball.vel.x + (old->obj.rolling_ball.acc.x * time);
    new->obj.rolling_ball.vel.y = old->obj.rolling_ball.vel.y + (old->obj.rolling_ball.acc.y * time);

    if (new->obj.rolling_ball.vel.x * old->obj.rolling_ball.vel.x < 0){
        new->obj.rolling_ball.vel.x = 0;
        new->obj.rolling_ball.acc.x = 0;
    }
    if (new->obj.rolling_ball.vel.y * old->obj.rolling_ball.vel.y < 0){
        new->obj.rolling_ball.vel.y = 0;
        new->obj.rolling_ball.acc.y = 0;
    }
}
unsigned char phylib_stopped( phylib_object *object ){
    if (phylib_length(object->obj.rolling_ball.vel) < PHYLIB_VEL_EPSILON){
        object->type = PHYLIB_STILL_BALL;
        object->obj.still_ball.number = object->obj.rolling_ball.number;
        object->obj.still_ball.pos = object->obj.rolling_ball.pos;
        return 1;
    } else {
        return 0;
    }
}
void phylib_bounce( phylib_object **a, phylib_object **b ){
    switch((*b)->type){
        case PHYLIB_HCUSHION: {
            double temp;
            temp = (*a)->obj.rolling_ball.vel.y;
            (*a)->obj.rolling_ball.vel.y = (*a)->obj.rolling_ball.acc.y;
            (*a)->obj.rolling_ball.acc.y = temp;
            break;
        }
        case PHYLIB_VCUSHION: {
            double temp;
            temp = (*a)->obj.rolling_ball.vel.x;
            (*a)->obj.rolling_ball.vel.x = (*a)->obj.rolling_ball.acc.x;
            (*a)->obj.rolling_ball.acc.x = temp;
            break;
        }
        case PHYLIB_HOLE:
            free((*a));
            (*a) = NULL;
            break;
        case PHYLIB_STILL_BALL:
            (*b)->type = PHYLIB_ROLLING_BALL;
        case PHYLIB_ROLLING_BALL: {
            phylib_coord r_ab = phylib_sub((*a)->obj.rolling_ball.pos, (*b)->obj.rolling_ball.pos);
            phylib_coord v_rel = phylib_sub((*a)->obj.rolling_ball.vel, (*b)->obj.rolling_ball.vel);
            phylib_coord n = {r_ab.x / phylib_length(r_ab), r_ab.y / phylib_length(r_ab)};
            double vel_rel_n = phylib_dot_product(v_rel, n);
            (*a)->obj.rolling_ball.vel.x -= vel_rel_n * n.x;
            (*a)->obj.rolling_ball.vel.y -= vel_rel_n * n.y;
            (*b)->obj.rolling_ball.vel.x += vel_rel_n * n.x;
            (*b)->obj.rolling_ball.vel.y += vel_rel_n * n.y;
            double a_speed = phylib_length((*a)->obj.rolling_ball.vel);
            double b_speed = phylib_length((*b)->obj.rolling_ball.vel);
            if (a_speed > PHYLIB_VEL_EPSILON){
                (*a)->obj.rolling_ball.acc.x = -(*a)->obj.rolling_ball.vel.x / a_speed * PHYLIB_DRAG;
                (*a)->obj.rolling_ball.acc.y = -(*a)->obj.rolling_ball.vel.y / a_speed * PHYLIB_DRAG;
            }
            if (b_speed > PHYLIB_VEL_EPSILON){
                (*b)->obj.rolling_ball.acc.x = -(*b)->obj.rolling_ball.vel.x / a_speed * PHYLIB_DRAG;
                (*b)->obj.rolling_ball.acc.y = -(*b)->obj.rolling_ball.vel.y / a_speed * PHYLIB_DRAG;
            }
            break;
        }
    }
}
unsigned char phylib_rolling( phylib_table *t ){
    unsigned char num_rolling = 0;
    for (int i = 0; i < PHYLIB_MAX_OBJECTS; i++){
        if (t->object[i] != NULL && t->object[i]->type == PHYLIB_ROLLING_BALL){
            num_rolling++;
        }
    }
    return num_rolling;
}
phylib_table *phylib_segment( phylib_table *table ){
    if (table == NULL){
        return NULL;
    }
    if (phylib_rolling(table) == 0){
        return NULL;
    }
    phylib_table * new_table = phylib_copy_table(table);

    while (new_table->time < PHYLIB_MAX_TIME){
        for (int i = 0; i < PHYLIB_MAX_OBJECTS; i++){
            if (new_table->object[i] != NULL && new_table->object[i]->type == PHYLIB_ROLLING_BALL){
                phylib_object *ball = new_table->object[i];
                if (phylib_stopped(ball)){
                    return new_table;
                }
                phylib_object * new_ball = NULL;
                phylib_copy_object(&new_ball, &ball);
                phylib_roll(new_ball, ball, PHYLIB_SIM_RATE);
                free(ball);
                //phylib_print_object(new_ball);
                
                phylib_copy_object(&new_table->object[i], &new_ball);
                free(new_ball);

                for (int j = 0; j < PHYLIB_MAX_OBJECTS; j++){
                    if (new_table->object[j] != NULL){
                        double distance = phylib_distance(new_table->object[i], new_table->object[j]);
                        if (new_table->object[i] != new_table->object[j]){
                            if (distance < 0.0){
                                phylib_bounce(&new_table->object[i],&new_table->object[j]);
                                return new_table;
                            }
                        }
                    }
                }
                //free(new_ball);
            }
        }
        new_table->time += PHYLIB_SIM_RATE;
        //printf("THE FUCKING TIME IS %lf\n", new_table->time );
    }
    //new_table->time += time;
    return new_table;
}
// phylib_table *phylib_segment(phylib_table *table){
//     if (table == NULL){
//         return NULL;
//     }
//     if (phylib_rolling(table) == 0){
//         return NULL;
//     }
//     phylib_table *copy_table = phylib_copy_table(table);
//     while (copy_table->time < PHYLIB_MAX_TIME){
//         for (int i = 0; i < PHYLIB_MAX_OBJECTS; i++){
//             //fprintf( stderr, "%s %s %d\n", __FILE__, __func__, __LINE__ );
//             if (copy_table->object[i] != NULL && copy_table->object[i]->type == PHYLIB_ROLLING_BALL){
//                 //fprintf( stderr, "%s %s %d\n", __FILE__, __func__, __LINE__ );
//                 if (phylib_stopped(copy_table->object[i])){
//                     //fprintf( stderr, "%s %s %d\n", __FILE__, __func__, __LINE__ );
//                     return copy_table;
//                 }
//                 phylib_object *new_ball = NULL;
//                 phylib_copy_object(&new_ball,&copy_table->object[i]);
//                 //phylib_print_object(copy_table->object[i]);
//                 phylib_roll(new_ball,copy_table->object[i],PHYLIB_SIM_RATE);
//                 //phylib_print_object(new_ball);
//                 //printf("%lf\n",copy_table->time);
//                 phylib_copy_object(&copy_table->object[i], &new_ball);
//                 //phylib_print_object(copy_table->object[i]);
//                 //fprintf( stderr, "%s %s %d\n", __FILE__, __func__, __LINE__ );
//                 free(new_ball);
//                 //fprintf( stderr, "%s %s %d\n", __FILE__, __func__, __LINE__ );
//                 for (int j = 0; j < PHYLIB_MAX_OBJECTS; j++){
//                     //fprintf( stderr, "%s %s %d\n", __FILE__, __func__, __LINE__ );
//                     double distance = phylib_distance(copy_table->object[i], copy_table->object[j]);
//                     //fprintf( stderr, "%s %s %d\n", __FILE__, __func__, __LINE__ );
//                     if (distance < 0.0 && copy_table->object[i] != copy_table->object[j] && distance != -1){
//                         //fprintf( stderr, "%s %s %d\n", __FILE__, __func__, __LINE__ );
//                         phylib_bounce(&copy_table->object[i],&copy_table->object[j]);
//                         //fprintf( stderr, "%s %s %d\n", __FILE__, __func__, __LINE__ );
//                         return copy_table;
//                 }
//                 //fprintf( stderr, "%s %s %d\n", __FILE__, __func__, __LINE__ );
//             }
//             }
//         }
//         //fprintf( stderr, "%s %s %d\n", __FILE__, __func__, __LINE__ );
//         copy_table->time += PHYLIB_SIM_RATE;
//         //fprintf( stderr, "%s %s %d\n", __FILE__, __func__, __LINE__ );
//     }
//     //printf("END\n\n");
//     return copy_table;
// }
