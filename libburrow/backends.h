#ifndef __BURROW_BACKENDS_H
#define __BURROW_BACKENDS_H

#ifdef  __cplusplus
extern "C" {
#endif

burrow_backend_functions_st *burrow_backend_load_functions(const char *backend);

#ifdef  __cplusplus
}
#endif

#endif /* __BURROW_BACKENDS_H */