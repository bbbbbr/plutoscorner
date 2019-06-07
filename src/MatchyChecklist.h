//
// MatchyChecklist.h
//

#define CHECKLIST_INIT        0
#define CHECKLIST_DISHES_DONE 1
#define CHECKLIST_KIBBLE_DONE 2
#define CHECKLIST_MOTHS_DONE  3
#define CHECKLIST_PLUTO_DONE  4
#define CHECKLIST_NAP_DONE    5

extern UINT8 matchy_checklist_status;

#define MATCHY_CHECKLIST_INCREMENT matchy_checklist_status++