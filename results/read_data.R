# read csv assignment 2

source("./performance_functions.R")
setwd("../results/")


###################
### results on GPU 
###################

#######################
### strong scalability
#######################

df_strong_gpu <- data.frame(read.csv("strong_scaling_gpu.csv"))

### plot results wrt openmp
  plot_times(df_strong_gpu, "strong_gpu")
  #speedup -> Amdahl's law
  plot_speedup(df_strong_gpu, "strong_gpu")
  
  #efficiency
  plot_efficiency(df_strong_gpu)
  
  #Model
  model(df_strong_gpu, "strong_gpu")

### plot results wrt MPI
  plot_times_mpi(df_strong_gpu, "strong_gpu_mpi")
  plot_speedup_mpi(df_strong_gpu)
  
  model_mpi(df_strong_gpu, "strong_gpu_mpi")




#####################
### weak scalability
#####################

df_weak_gpu <- data.frame(read.csv("weak_scaling_gpu.csv"))

### plot results wrt openmp  
  plot_times(df_weak_gpu, "weak_gpu")

  #speedup -> Amdahl's law
  plot_speedup_weak(df_weak_gpu, "weak_gpu")

### plot results wrt mpi
  plot_times_mpi(df_weak_gpu, "weak_gpu_mpi")
  
  #speedup -> Amdahl's law
  plot_speedup_weak_mpi(df_weak_gpu, "weak_gpu_mpi")

  
###################
### results on CPUs
###################

#######################
### strong scalability
#######################

df_strong_cpu <- data.frame(read.csv("strong_scaling_cpu.csv"))

### plot results wrt openmp  
  
  #times 
  plot_times(df_strong_cpu, "strong_cpu")
  
  #speedup -> Amdahl's law
  plot_speedup(df_strong_cpu, "strong_cpu")
  
  #efficiency
  plot_efficiency(df_strong_cpu)
  
  #Model
  model(df_strong_cpu, "strong_cpu")

### plot results wrt MPI
  plot_times_mpi(df_strong_cpu, "strong_cpu_mpi")
  plot_speedup_mpi(df_strong_cpu)
  
  model_mpi(df_strong_cpu, "strong_cpu_mpi")
  
  model_omp_mpi(df_strong_cpu, "strong_cpu_omp+mpi")
  
#####################
### weak scalability
#####################

df_weak_cpu <- data.frame(read.csv("weak_scaling_cpu.csv"))

### plot results wrt openmp  
  
  #times 
  plot_times(df_weak_cpu, "weak_cpu")
  
  #speedup -> Amdahl's law
  plot_speedup_weak(df_weak_cpu,"weak_cpu")
  
  
### plot results wrt MPI  

  #times 
  plot_times_mpi(df_weak_cpu, "weak_cpu_mpi")
  
  #speedup -> Amdahl's law
  plot_speedup_weak_mpi(df_weak_cpu,"weak_cpu_mpi")
  

  
#######

# different results obtain with and without parallelism on first 
# k splits partitioning, compare results obtained using 4 MPI processes

df_strong_cpu_parallel <- data.frame(read.csv("first_ksplit_parallel.csv"))
df <- df_strong_cpu_parallel  
df1 <- df_strong_cpu
df4 <- df[which(df$n_procs == 4), ]
df41 <- df1[which(df1$n_procs == 4), ]

### times
ggplot() +

  geom_line(data = df4, aes(x = as.factor(n_threads), y = time_taken, color="parallel partitioning", group=1)) +
  geom_point(data = df4, aes(x = as.factor(n_threads), y = time_taken, color="parallel partitioning", group = 1))  + 
  
  geom_line(data = df41, aes(x = as.factor(n_threads), y = time_taken, color="serial partitioning", group=1)) +
  geom_point(data = df41, aes(x = as.factor(n_threads), y = time_taken, color="serial partitioning", group = 1))  + 

  labs(x = "num threads", y = "Time taken (s)") +
  theme(legend.title = element_blank()) +
  scale_colour_manual(values = c( "parallel partitioning" = "#d16206", "serial partitioning" = "#1b572c")) +
  labs(title = "")

#ggsave("img/comparison_parallel_partitioning.png", width = 10, height = 8, dpi = 150)

### speedup
ggplot() +

  geom_line(data = df4, aes(x = as.factor(n_threads), y = df4[1,]$time_taken/time_taken, color="parallel partitioning", group=1)) +
  geom_point(data = df4, aes(x = as.factor(n_threads), y = df4[1,]$time_taken/time_taken, color="parallel partitioning", group = 1))  + 
  
  geom_line(data = df41, aes(x = as.factor(n_threads), y = df41[1,]$time_taken/time_taken, color="serial partitioning", group=1)) +
  geom_point(data = df41, aes(x = as.factor(n_threads), y = df41[1,]$time_taken/time_taken, color="serial partitioning", group = 1))  + 
  
  labs(x = "num threads", y = "Speedup") +
  theme(legend.title = element_blank()) +
  scale_colour_manual(values = c( "parallel partitioning" = "#d16206", "serial partitioning" = "#1b572c")) +
  labs(title = "")
  
  ggsave("img/comparison_parallel_partitioning_speedup.png", width = 10, height = 8, dpi = 150)
  
  
### 3d plot of cpu (strong scalability) timings
  
library(rgl)

x <- df_strong_cpu$n_procs
y <- df_strong_cpu$n_threads
z <- df_strong_cpu$time_taken

require(akima)
require(rgl)

s=interp(x,y,z,duplicate="strip")

axx <- list(title = "mpi procs")
axy <- list(title = "threads")
axz <- list(title = "time")

library(plotly)
fig <- plot_ly(x = s$mpi_procs, y = s$y, z = s$z) %>%  add_surface()  %>% layout(scene = list(xaxis=axx,yaxis=axy,zaxis=axz))

fig

