// Replacement file to provide atomic instruction dummies
#ifndef CMSIS_COMPILER_DUMMY_H
#define CMSIS_COMPILER_DUMMY_H

#define __enable_irq() do{}while(0)
#define __disable_irq() do{}while(0)
#define __get_PRIMASK() 0
#define __set_PRIMASK(primask)

#endif