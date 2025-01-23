#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


int main ()
{
    int quater,score,opp_score,down,distance,return6_temp,score_after_temp, score_result, yards;
    bool return6,redzone,score_after;
    float time_in_quater;
    printf("Welcome to the Turnover index! Please enter all the info below.\n");
    printf("Enter quater: ");
    scanf("%d", &quater);
    printf("Enter the time(Enter as a decimal):");
    scanf("%f", &time_in_quater);
    printf("Enter score and opp. score: ");
    scanf("%d %d", &score,&opp_score);
    printf("Enter down and distance: ");
    scanf("%d %d", &down, &distance);
    printf("Where on the field did the turnover occur (In reference to the team's endzone ie.redzone would be 80+)?");
    scanf("%d", &yards);
    printf("Was the TO a returned for points or did they score on that drive?(Enter 1 or 0 for both) ");
    scanf("%d %d", &return6_temp, &score_after_temp);
    if (score_after_temp == 1)
    {
        printf("Did they score a touchdown (1) or field goal(0)? ");
        scanf("%d", &score_result);
    }

    float yard_impact;
    //getting yards impact, first 30 is penlized more than the 31+ yardage
    if (yards >= 31)
    {
       yard_impact = ((100-yards)/(pow(yards,.5)))+1; 
    }
    else if (yards <= 30)
    {
    yard_impact = -0.5*yards +30;
    }

    //tagging if TO was in redzone
    if (yards >= 80)
    {
        redzone = 1;
    }
    else
    {
        redzone = 0;
    }

    //getting score differences
    int score_diff = score - opp_score;
    //if score is greater than 10, treat as the same
    if (score_diff >= 10)
    {
        score_diff = 10;
    }
    //same as above but in the oppisite way
    else if (score_diff <= -10)
    {
        score_diff = -10;
    }

    //assigning booleans values
    return6 = (return6_temp==1);
    score_after = (score_after_temp==1);

  

    float score_impact;
    float return_score_importance;

    switch (score_diff)                         //weights for score difference
    {
        case 0:
        score_impact = 2.5;
        break;
        case 1:
        score_impact = 2.25;
        break;
        case 2: 
        score_impact = 2.0;
        break;
        case 3:
        score_impact = 1.8;
        break;
        case 4:
        score_impact = 1.6;
        break;
        case 5:
        score_impact = 1.4;
        break;
        case 6:
        score_impact = 1.2;         //review
        break;
        case 7:
        score_impact = 1.2;
        break;
        case 8:
        score_impact = 1.2;
        break;
        case 9:
        score_impact = 1.0;
        break;
        case 10:
        score_impact = 1.0;
        break;

        case -1:
        score_impact = 3.0;
        break;
        case -2:
        score_impact = 4.0;
        break;
        case -3:
        score_impact = 4.5;
        break;
        case -4: 
        score_impact = 5.5;
        break;
        case -5:
        score_impact = 6.5;
        break;
        case -6:
        score_impact = 7.5;
        break;
        case -7:
        score_impact = 7.5;
        break;
        case -8:
        score_impact = 8.5;
        break;
        case -9:
        score_impact = 9.0;
        break;
        case -10:
        score_impact = 10.0;
        break;
    }

    //total time used as multipler since its a natural scale
    float time_remianing = 60-((60.00-quater*15) + time_in_quater);
    float time_importance = time_remianing;        
    float down_importance;

    //if distance is greater than 10 yards
    if (distance >= 10)
    {
        distance = 10;
    }

    //assign weight based on down and distance
    switch (down)
    {
        case 1:
        down_importance= down * (11-distance) *1.00;
        break;

        case 2:
        down_importance = down * (11-distance) *.75;
        break;

        case 3:
        down_importance = down * (11-distance) *.50;
        break;
        
        case 4:
        down_importance = down * (11-distance) *.25;
        break;
    }

    //return score importance (based on outcome)
    if (return6 == 1)
    {
       return_score_importance = 3.0;
    } 
    else if (score_after == 1)
    {
        if (score_result == 1)
        {
           return_score_importance = 2.0; 
        }
        else if (score_result == 0)
        {
            return_score_importance = 1.5;
        }
        
    }
    else if (return6 ==0 && score_after ==0)
    {
        return_score_importance = 1.0;
    }


    //redzone impact (assigning impact based on score outcome)
    float redzone_impact = 0.0;
    if (redzone ==1)
    {
        if (return6 ==1)
        {
            redzone_impact = 3.0;
        }
        else
        {
            if (score_after == 1)
            {
                redzone_impact = 2.5;
            }
            else
            {
                redzone_impact = 2.0;
            }
        }

        if (score_diff <= -7)
        {
            redzone_impact += 2.0;
        }
        else if (score_diff >= -3 && score_diff <= 0)
        {
            redzone_impact +=1.5;
        }
    }
    else if (redzone == 0)
    {
        redzone_impact = 1.0;
    }

    //Scoring after the turnover (adding score impact based on outcome)
    if ((score_diff <= 7 && score_diff >= 0) && return6 == 1)
    {
        score_impact += 5.0;
        
    }
    else if ((score_diff <= 7 && score_diff >= 0) && score_result == 1)
    {
        score_impact += 3.0;
    }
    else if ((score_diff <= 7 && score_diff >= 0) && score_result == 0)
    {
        score_impact += 1.0;
    }

    //if the ball is turned over but the opp. team doesnt score and your still winning, add a "reward"
    if (return_score_importance ==1.0 && (score_diff > 0 && score_diff<3 ))    //tied to 2 point lead
    {
        return_score_importance = -1.0;
    }
    else if (return_score_importance == 1.0 && (score_diff >= 3 && score_diff <=8)) //winnning by 3 to 8
    {
        return_score_importance = -2.0;
    }
    else if (return_score_importance == 1.0 && score_diff > 8) //winning by 2 scores
    {
        return_score_importance = -3.0;
    }
    
    printf("\nTime impact: %.2f",time_importance); 
    printf("\nScore impact: %.2f", score_impact);
    printf("\nReturn impact: %.2f",return_score_importance);
    printf("\nYard impact: %.2f ", yard_impact);
    printf("\nRedzone impact: %.2f",redzone_impact);
    printf("\nDown impact: %.2f",down_importance);

    float TO_index = time_importance+score_impact*return_score_importance+yard_impact+redzone_impact+down_importance;
    printf("\n-------------------------\nYour To Index is: %.2f", TO_index);

    return 0;
}