extern debug bug;
extern int i;

// type definition for defining any number of values to track and clean
typedef struct lp{
    float beta;  // 4 bytes, constant that determines percent difference to clean
    float value[100];  // 200 bytes, saves last 50 values
    int index;  // 4 bytes, index index that loops
    float avg;  // 4 bytes, created for a running average
    int name; // MUST BE DEFINED IN get_name() AS REGULAR ASSIGMENT DOESN'T WORK
}lp; // 212 bytes per

// initialize data types (lp* type, float beta, int name)
void init_lp(lp* lp_struct, float beta, int name){
    //lp_struct = (lp*)malloc(sizeof(lp));
    lp_struct->index = 0; // zero index
    lp_struct->avg = 0.0f; // zero avg
    lp_struct->beta = beta; // assign desired beta
    lp_struct->name = name; // assign desired int value that defines name in get_name(int name)
    for(int i = 0; i <100; i++){  
      lp_struct->value[i] = 0.0f; // zero all values 
    }
}

// define name based on an int because arduinos do not like char* in a struct
char* get_name(int name){
    if(name == 0){
        return "pitch";
    }else if(name == 1){
        return "roll";
    }else if(name == 2){
        return "gx";
    }else if(name == 3){
        return "gy";
    }else{
        return "no name given";
    }
}

float low_pass(lp* lp_struct, float val){
    int num_values = 100;
    float num_values_f = 100.0f;
    
    // reset index to loop (therefore this runs on the last 50 values averaged)
    if(lp_struct->index >= num_values-1){
        lp_struct->index = -1;
    }

    lp_struct->index += 1;
    if(bug.low_pass_spam){
        Serial.print(lp_struct->index);
        Serial.print("\t name: ");
        Serial.print(lp_struct->name);
        Serial.print("\t beta: ");
        Serial.println(lp_struct->beta);

    }

    // update the running average
    lp_struct->avg += val/(num_values_f);
    lp_struct->avg -= lp_struct->value[lp_struct->index]/(num_values_f);
    
    // replace the current index position with the new one
    lp_struct->value[lp_struct->index] = val;
    if(bug.low_pass_spam){
        Serial.print("added value: ");
        Serial.print(val/(num_values_f));
        Serial.print("\t removed value");
        Serial.println(lp_struct->value[lp_struct->index]/(num_values_f));
    }

    // check if value is within beta percentage of average, limit values
    if (val/lp_struct->avg > (1+lp_struct->beta)){
        if(bug.low_pass & (lp_struct->index == num_values-1)){
            Serial.println("low_pass ---------------------------");
            Serial.print(get_name(lp_struct->name));
            Serial.println("...");
            Serial.print(".. Instant Value: ");
            Serial.print(val);
            Serial.print(".. Average Value: ");
            Serial.print(lp_struct->avg);
            Serial.println(".. This value was upper bounded");
        }
        val = (1+lp_struct->beta)*lp_struct->avg;
    } else if (val/lp_struct->avg < (1-lp_struct->beta)){
        if(bug.low_pass & (lp_struct->index == num_values-1)){
            Serial.println("low_pass ---------------------------");
            Serial.print(get_name(lp_struct->name));
            Serial.println("...");
            Serial.print(".. Instant Value: ");
            Serial.print(val);
            Serial.print(".. Average Value: ");
            Serial.print(lp_struct->avg);
            Serial.println(".. This value was lower bounded");
        }
        val = (1-lp_struct->beta)*lp_struct->avg;
    }
    
    if(bug.low_pass & (lp_struct->index == num_values-1)){
        Serial.println("low_pass ---------------------------");
        Serial.print(get_name(lp_struct->name));
        Serial.println("...");
        Serial.print(".. Instant Value: ");
        Serial.print(val);
        Serial.print(".. Average Value: ");
        Serial.print(lp_struct->avg);
        Serial.println(".. This value was not bounded");
    }

    if(bug.low_pass_spam){
        Serial.print("name: "); Serial.print(get_name(lp_struct->name));
        Serial.print("\t index: "); Serial.print(lp_struct->index);
        Serial.print("\t Average: "); Serial.print(lp_struct->avg);
        Serial.print("\t value: "); Serial.println(val);
    }

    return val;
}