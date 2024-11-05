#ifndef STUB_H_INCLUDED
#define STUB_H_INCLUDED

extern void gdb_stub_init(void);
extern void gdb_stub_deinit(void);

void *task_gdb_stub( void *ignore );

#endif // STUB_H_INCLUDED
