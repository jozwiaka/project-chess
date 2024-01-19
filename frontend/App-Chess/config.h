#ifndef CONFIG_H
#define CONFIG_H
#define PVP
// #define PVC
// #define CVC

#if !defined(PVP) && !defined(PVC) && !defined(CVC)
#error "At least one of PVP, PVC, or CVC must be defined."
#endif
#endif // CONFIG_H
