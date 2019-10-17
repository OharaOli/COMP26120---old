#include <stdio.h>
#include <string.h>

//Convert the temperature from Celsius to Farenheit
float c2f (float c)
{
    return (1.8*c + 32);
}

//Convert the temperature from Fahrenheit to Celsius
float f2c (float f)
{
    return (((f-32)*5)/9);
}

//main method
int main(int argc, char *argv[]) {
   
    //Variable used to store the temperature
    float temperature;

    //Check for an incorrect number of inputs
    if(argc <= 2 || argc > 3) 
    {
        printf("Error: The wrong number of parameters have been supplies\n");
        return 1;
    }
    //Check for lack of -f or -c
    else if(strcmp (argv[1],"-f") != 0 && strcmp (argv[1],"-c") != 0)
    {
        printf("Error: Failed to supply -f or -c as the first option\n");
        return 2;
    }
    //Check that the number given is a valid float
    //else if(sscanf(argv[2], "%f%*[^\n]", &temperature) != 1)
    else if(sscanf(argv[2], "%f", &temperature) != 1)
    {
        printf("Error: Input is not a valid float\n");
        return 3;
    }
    //Case for -f
    else if(strcmp (argv[1],"-f") == 0)
    {
        //Error if the temperature is below absolute zero
        if(temperature < -459.67)
        {
            printf("Error: Temperature below absolute zero\n");
            return 4;
        }
        //Print conversion
        printf("%.2f°C = %.2f°F\n", f2c(temperature), temperature);
    }
    //Case for -c
    else if(strcmp (argv[1],"-c") == 0)
    {
        //Error if the temperature is below absolute zero
        if(temperature < -273.15)
        {
            printf("Error: Temperature below absolute zero\n");
            return 4;
        }
        //Print conversion
        printf("%.2f°C = %.2f°F\n", temperature, c2f(temperature));
    }
}

