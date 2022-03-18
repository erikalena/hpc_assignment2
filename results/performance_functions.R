library(ggplot2)
library(RColorBrewer)
library(patchwork)
library(data.table)
library(ggpubr)

col_legend <- brewer.pal(n=8, name="Dark2")

##############
#functions to plot different performances metrics

plot_times <- function(df,name) {
  df_1_procs <- df[which(df$n_procs == 1), ]
  df_2_procs <- df[which(df$n_procs == 2), ]
  df_4_procs <- df[which(df$n_procs == 4), ]
  df_8_procs <- df[which(df$n_procs == 8), ]
  df_16_procs <- df[which(df$n_procs == 16), ]
  
  # times 
  ggplot() +
    geom_line(data = df_1_procs, aes(x = as.factor(n_threads), y = time_taken, color="1 procs", group=1)) +
    geom_point(data = df_1_procs, aes(x = as.factor(n_threads), y = time_taken, color="1 procs", group = 1))  + 
    
    geom_line(data = df_2_procs, aes(x = as.factor(n_threads), y = time_taken, color="2 procs", group=1)) +
    geom_point(data = df_2_procs, aes(x = as.factor(n_threads), y = time_taken, color="2 procs", group = 1))  + 
    
    geom_line(data = df_4_procs, aes(x = as.factor(n_threads), y = time_taken, color="4 procs", group=1)) +
    geom_point(data = df_4_procs, aes(x = as.factor(n_threads), y = time_taken, color="4 procs", group = 1))  + 
    
    geom_line(data = df_8_procs, aes(x = as.factor(n_threads), y = time_taken, color="8 procs", group=1)) +
    geom_point(data = df_8_procs, aes(x = as.factor(n_threads), y = time_taken, color="8 procs", group = 1))  + 
    
    geom_line(data = df_16_procs, aes(x = as.factor(n_threads), y = time_taken, color="16 procs", group=1)) +
    geom_point(data = df_16_procs, aes(x = as.factor(n_threads), y = time_taken, color="16 procs", group = 1))  + 
    
    
    labs(x = "num threads", y = "Time taken (s)") +
    theme(legend.title = element_blank()) +
    scale_colour_manual(values = c("1 procs" = "#a6620f", "2 procs" = "#2bacbd", "4 procs" = "#8f34eb", "8 procs" = "#1b572c", "16 procs" = "#e1e809" )) +
    labs(title = "")+
    theme(axis.text = element_text(size = rel(1.2)), axis.title.x = element_text(size = rel(1.5)), axis.title.y = element_text(size = rel(1.2)),
          title = element_text(size = rel(1.5)),legend.text = element_text(size=15) )
  
  #ggsave(paste0( "img/times_", name, ".png"), width = 12, height = 8, dpi = 150)
  
}

plot_speedup <- function(df, name) {
  df_1_procs <- df[which(df$n_procs == 1), ]
  df_2_procs <- df[which(df$n_procs == 2), ]
  df_4_procs <- df[which(df$n_procs == 4), ]
  df_8_procs <- df[which(df$n_procs == 8), ]
  df_16_procs <- df[which(df$n_procs == 16), ]
  
  
  ggplot() +
    geom_line(data = df_1_procs, aes(x = as.factor(n_threads), y =  df[1,]$time_taken/time_taken, color="1 procs", group=1)) +
    geom_point(data = df_1_procs, aes(x = as.factor(n_threads), y =  df[1,]$time_taken/time_taken, color="1 procs", group = 1))  + 
    
    geom_line(data = df_2_procs, aes(x = as.factor(n_threads), y = df_2_procs[1,]$time_taken/time_taken, color="2 procs", group=1)) +
    geom_point(data = df_2_procs, aes(x = as.factor(n_threads), y = df_2_procs[1,]$time_taken/time_taken, color="2 procs", group = 1))  + 
    
    geom_line(data = df_4_procs, aes(x = as.factor(n_threads), y = df_4_procs[1,]$time_taken/time_taken, color="4 procs", group=1)) +
    geom_point(data = df_4_procs, aes(x = as.factor(n_threads), y = df_4_procs[1,]$time_taken/time_taken, color="4 procs", group = 1))  + 
    
    geom_line(data = df_8_procs, aes(x = as.factor(n_threads), y = df_8_procs[1,]$time_taken/time_taken, color="8 procs", group=1)) +
    geom_point(data = df_8_procs, aes(x = as.factor(n_threads), y = df_8_procs[1,]$time_taken/time_taken, color="8 procs", group = 1))  + 
    
    geom_line(data = df_16_procs, aes(x = as.factor(n_threads), y = df_16_procs[1,]$time_taken/time_taken, color="16 procs", group=1)) +
    geom_point(data = df_16_procs, aes(x = as.factor(n_threads), y = df_16_procs[1,]$time_taken/time_taken, color="16 procs", group = 1))  + 
    
    
    labs(x = "num threads", y = "Speedup") +
    theme(legend.title = element_blank()) +
    scale_colour_manual(values = c("1 procs" = "#a6620f", "2 procs" = "#2bacbd", "4 procs" = "#8f34eb", "8 procs" = "#1b572c", "16 procs" = "#e1e809" )) +
    labs(title = "")+
    theme(axis.text = element_text(size = rel(1.2)), axis.title.x = element_text(size = rel(1.5)), axis.title.y = element_text(size = rel(1.2)),
          title = element_text(size = rel(1.5)),legend.text = element_text(size=15) )
  
  #ggsave(paste0( "img/speedup_", name, ".png"), width = 12, height = 8, dpi = 150)
  
}

plot_speedup_weak <- function(df, name) {
  df_1_procs <- df[which(df$n_procs == 1), ]
  df_2_procs <- df[which(df$n_procs == 2), ]
  df_4_procs <- df[which(df$n_procs == 4), ]
  df_8_procs <- df[which(df$n_procs == 8), ]
  df_16_procs <- df[which(df$n_procs == 16), ]
  
  s <- (0.018*df$problem_size*10^(-6)) # intrinsically sequential time 
  sp <- s/df$time_taken #fraction of sequential code execution on the whole time taken
  
  n <- nrow(df_1_procs)
  
  ggplot() +
    geom_line(data = df_1_procs, aes(x = as.factor(n_threads), y = n_threads + sp[1:n]*(1-n_threads), color="1 procs", group=1)) +
    geom_point(data = df_1_procs, aes(x = as.factor(n_threads), y = n_threads + sp[1:n]*(1-n_threads), color="1 procs", group = 1))  + 
    
    geom_line(data = df_2_procs, aes(x = as.factor(n_threads), y = n_threads + sp[(n+1):(n*2)]*(1-n_threads), color="2 procs", group=1)) +
    geom_point(data = df_2_procs, aes(x = as.factor(n_threads), y = n_threads + sp[(n+1):(n*2)]*(1-n_threads), color="2 procs", group = 1))  +
    
    geom_line(data = df_4_procs, aes(x = as.factor(n_threads), y = n_threads + sp[(n*2+1):(n*3)]*(1-n_threads), color="4 procs", group=1)) +
    geom_point(data = df_4_procs, aes(x = as.factor(n_threads), y = n_threads + sp[(n*2+1):(n*3)]*(1-n_threads), color="4 procs", group = 1))  +
    
    geom_line(data = df_8_procs, aes(x = as.factor(n_threads), y = n_threads + sp[(n*3+1):(n*4)]*(1-n_threads), color="8 procs", group=1)) +
    geom_point(data = df_8_procs, aes(x = as.factor(n_threads), y = n_threads + sp[(n*3+1):(n*4)]*(1-n_threads), color="8 procs", group = 1))  +
    
    geom_line(data = df_16_procs, aes(x = as.factor(n_threads), y = n_threads + sp[(n*4+1):(n*5)]*(1-n_threads), color="16 procs", group=1)) +
    geom_point(data = df_16_procs, aes(x = as.factor(n_threads), y = n_threads + sp[(n*4+1):(n*5)]*(1-n_threads), color="16 procs", group = 1))  +
    
    
    labs(x = "num threads", y = "Speedup") +
    theme(legend.title = element_blank()) +
    scale_colour_manual(values = c("1 procs" = "#a6620f", "2 procs" = "#2bacbd", "4 procs" = "#8f34eb", "8 procs" = "#1b572c", "16 procs" = "#e1e809" )) +
    labs(title = "")
  
  #ggsave(paste0( "img/speedup_", name, ".png"), width = 12, height = 8, dpi = 150)
  
}

plot_efficiency <- function(df) {
  df_1_procs <- df[which(df$n_procs == 1), ]
  df_2_procs <- df[which(df$n_procs == 2), ]
  df_4_procs <- df[which(df$n_procs == 4), ]
  df_8_procs <- df[which(df$n_procs == 8), ]
  df_16_procs <- df[which(df$n_procs == 16), ]
  
  ggplot() +
    # theoretical 
    geom_line(data = df_1_procs, aes(x = as.factor(n_threads), y =  df_1_procs[1,]$time_taken/(1*n_threads*time_taken), color="1 procs", group=1)) +
    geom_point(data = df_1_procs, aes(x = as.factor(n_threads), y =  df_1_procs[1,]$time_taken/(1*n_threads*time_taken), color="1 procs", group = 1))  + 
    
    geom_line(data = df_2_procs, aes(x = as.factor(n_threads), y = df_2_procs[1,]$time_taken/(2*n_threads*time_taken), color="2 procs", group=1)) +
    geom_point(data = df_2_procs, aes(x = as.factor(n_threads), y = df_2_procs[1,]$time_taken/(2*n_threads*time_taken), color="2 procs", group = 1))  + 
    
    geom_line(data = df_4_procs, aes(x = as.factor(n_threads), y = df_4_procs[1,]$time_taken/(4*n_threads*time_taken), color="4 procs", group=1)) +
    geom_point(data = df_4_procs, aes(x = as.factor(n_threads), y = df_4_procs[1,]$time_taken/(4*n_threads*time_taken), color="4 procs", group = 1))  + 
    
    geom_line(data = df_8_procs, aes(x = as.factor(n_threads), y = df_8_procs[1,]$time_taken/(8*n_threads*time_taken), color="8 procs", group=1)) +
    geom_point(data = df_8_procs, aes(x = as.factor(n_threads), y = df_8_procs[1,]$time_taken/(8*n_threads*time_taken), color="8 procs", group = 1))  + 
    
    geom_line(data = df_16_procs, aes(x = as.factor(n_threads), y = df_16_procs[1,]$time_taken/(16*n_threads*time_taken), color="16 procs", group=1)) +
    geom_point(data = df_16_procs, aes(x = as.factor(n_threads), y = df_16_procs[1,]$time_taken/(16*n_threads*time_taken), color="16 procs", group = 1))  + 
    
    
    labs(x = "num threads", y = "Efficiency") +
    theme(legend.title = element_blank()) +
    scale_colour_manual(values = c("1 procs" = "#a6620f", "2 procs" = "#2bacbd", "4 procs" = "#8f34eb", "8 procs" = "#1b572c", "16 procs" = "#e1e809" )) +
    labs(title = "") +
    theme(axis.text = element_text(size = rel(1.2)), axis.title.x = element_text(size = rel(1.5)), axis.title.y = element_text(size = rel(1.2)),
          title = element_text(size = rel(1.5)),legend.text = element_text(size=15) )
}

model <- function(df, name) {
  #serial times
  t_tree <- 0.64
  ts <-  df[1,]$time_taken
  t_ord <- ts - t_tree # ~ time to read the array * 3 * log(N)
  # with N = 10^7 -> t_ord ~ 0.06 *3*log(10^7)
  
  Ts_1 <- 0.06*3 # at least one sequential read
  
  # then parallel time is parallelizable part divided by the numb of workers
  # + sequential part + (Ts - Ts_1)/n_workers
  df_1_procs <- df[which(df$n_procs == 1), ]
  n <- nrow(df_1_procs)
  tp <- rep(0,n) 
  
  for (i in 1:n){
    tp[i] <- t_tree/i + Ts_1 + (t_ord - Ts_1)/i
  }
  
  ggplot() +
    geom_line(data = df_1_procs, aes(x = as.factor(n_threads), y = time_taken, color="empirical", group=1)) +
    geom_point(data = df_1_procs, aes(x = as.factor(n_threads), y = time_taken, color="empirical", group = 1))  + 
    
    geom_line(data = df_1_procs, aes(x = as.factor(n_threads), y = tp, color="model", group=1)) +
    geom_point(data = df_1_procs, aes(x = as.factor(n_threads), y = tp, color="model", group = 1))  + 
    
    labs(x = "num threads", y = "Time taken (s)") +
    theme(legend.title = element_blank()) +
    scale_colour_manual(values = c("empirical" = "#a6620f", "model" = "#2bacbd" )) +
    labs(title = "") +
    theme(axis.text = element_text(size = rel(1.2)), axis.title.x = element_text(size = rel(1.5)), axis.title.y = element_text(size = rel(1.2)),
          title = element_text(size = rel(1.5)),legend.text = element_text(size=15) )
  
  #ggsave(paste0( "img/model_", name, ".png"), width = 12, height = 8, dpi = 150)
  
}

plot_times_mpi <- function(df, name) {
  df1 <- df[which(df$n_threads == 1), ]
  df4 <- df[which(df$n_threads == 4), ]
  df8 <- df[which(df$n_threads == 8), ]
  
  ggplot() +
    geom_line(data = df1,aes(x = n_procs, y=time_taken,  color="1 thread")) +
    geom_point(data = df1, aes(x = n_procs, y=time_taken, color="1 thread"))+
    geom_line(data = df4,aes(x = n_procs, y=time_taken, color="4 threads")) +
    geom_point(data = df4, aes(x = n_procs, y=time_taken, color="4 threads"))+
    geom_line(data = df8,aes(x = n_procs, y=time_taken, color="8 threads")) +
    geom_point(data = df8, aes(x = n_procs, y=time_taken, color="8 threads"))+
    scale_colour_manual(values = c("1 thread" = "#a6620f", "4 threads" = "#2bacbd", "8 threads" =  "#1b572c")) +
    
    labs(x = "mpi procs", y = "Time taken (s)") +
    theme(legend.title = element_blank()) +
    labs(title = "") +
    theme(axis.text = element_text(size = rel(1.2)), axis.title.x = element_text(size = rel(1.5)), axis.title.y = element_text(size = rel(1.2)),
          title = element_text(size = rel(1.5)),legend.text = element_text(size=15) )
  
  #ggsave(paste0( "img/times_", name, ".png"), width = 12, height = 8, dpi = 150)
  
}

plot_speedup_mpi <- function(df, name) {
  df1 <- df[which(df$n_threads == 1), ]
  df4 <- df[which(df$n_threads == 4), ]
  df8 <- df[which(df$n_threads == 8), ]
  
  ggplot() +
    geom_line(data = df1, aes(x = n_procs, y =  df1[1,]$time_taken/time_taken, color="1 thread", group=1)) +
    geom_point(data = df1, aes(x = n_procs, y =  df1[1,]$time_taken/time_taken, color="1 thread", group = 1))  + 
    
    geom_line(data = df4, aes(x = n_procs, y = df4[1,]$time_taken/time_taken, color="4 threads", group=1)) +
    geom_point(data = df4, aes(x = n_procs, y = df4[1,]$time_taken/time_taken, color="4 threads", group = 1))  + 
    
    geom_line(data = df8, aes(x = n_procs, y = df8[1,]$time_taken/time_taken, color="8 threads", group=1)) +
    geom_point(data = df8, aes(x = n_procs, y = df8[1,]$time_taken/time_taken, color="8 threads", group = 1))  + 
    theme(legend.title = element_blank()) +
    scale_colour_manual(values = c("1 thread" = "#a6620f", "4 threads" = "#2bacbd", "8 threads" =  "#1b572c")) +
    labs(x = "mpi procs", y = "Speedup") +
    labs(title = "")+
    theme(axis.text = element_text(size = rel(1.2)), axis.title.x = element_text(size = rel(1.5)), axis.title.y = element_text(size = rel(1.2)),
          title = element_text(size = rel(1.5)),legend.text = element_text(size=15) )
  
  #ggsave(paste0( "img/speedup_", name, ".png"), width = 12, height = 8, dpi = 150)
  
}


plot_speedup_weak_mpi <- function(df, name) {
  s <- (0.018*df$problem_size*10^(-6)) # intrinsically sequential time 
  df$sp <- s/df$time_taken #fraction of sequential code execution on the whole time taken
  
  df1 <- df[which(df$n_threads == 1), ]
  df4 <- df[which(df$n_threads == 4), ]
  df8 <- df[which(df$n_threads == 8), ]
  
  ggplot() +
    geom_line(data = df1, aes(x = n_procs, y = n_procs  + sp*(1 - n_procs), color="1 thread", group=1)) +
    geom_point(data = df1, aes(x = n_procs, y = n_procs  + sp*(1 - n_procs), color="1 thread", group = 1))  + 
    
    geom_line(data = df4, aes(x = n_procs, y = n_procs  + sp* (1 - n_procs), color="4 threads", group=1)) +
    geom_point(data = df4, aes(x = n_procs, y = n_procs  + sp* (1 - n_procs), color="4 threads", group = 1))  + 
    
    geom_line(data = df8, aes(x = n_procs, y = n_procs  + sp* (1 - n_procs), color="8 threads", group=1)) +
    geom_point(data = df8, aes(x = n_procs, y = n_procs  + sp* (1 - n_procs), color="8 threads", group = 1))  + 
    theme(legend.title = element_blank()) +
    scale_colour_manual(values = c("1 thread" = "#a6620f", "4 threads" = "#8f34eb", "8 threads" =  "#1b572c")) +
    labs(x = "mpi procs", y = "Speedup") +
    labs(title = "")+
    theme(axis.text = element_text(size = rel(1.2)), axis.title.x = element_text(size = rel(1.5)), axis.title.y = element_text(size = rel(1.2)),
          title = element_text(size = rel(1.5)),legend.text = element_text(size=15) )
  
  #ggsave(paste0( "img/speedup_", name, ".png"), width = 12, height = 8, dpi = 150)
  
}

model_mpi <- function(df, name) {
  df1 <- df[which(df$n_threads == 1), ]
  
  #serial times
  t_tree <- 0.64 - 0.18
  ts <-  df1$time_taken[1]
  t_ord <- ts - t_tree # ~ time to read the array * 3 * log(N)
  # with N = 10^7 -> t_ord ~ 0.06 *3*log(10^7)
  
  Ts_1 <- 0.06*3 # at least one sequential read
  
  # then parallel time is parallelizable part divided by the numb of workers
  # + sequential part + (Ts - Ts_1)/n_workers
  n <- nrow(df1)
  tp <- rep(0,n) 
  
  for (i in 1:n){
    if (i == 1)
      tp[i] <- t_tree + Ts_1 + (t_ord - Ts_1) 
    else {
      p_size <- df1$problem_size[1]/df1$n_procs[i]
      tp[i] <- t_tree/df1$n_procs[i] + (0.06 *3*10^(-7)*p_size*log(p_size,2)) + df1$mpi_time[i]
    }
  }
  
  ggplot() +
    geom_line(data = df1, aes(x = as.factor(n_procs), y = time_taken, color="empirical", group=1)) +
    geom_point(data = df1, aes(x = as.factor(n_procs), y = time_taken, color="empirical", group = 1))  + 
    
    geom_line(data = df1, aes(x = as.factor(n_procs), y = tp, color="model", group=1)) +
    geom_point(data = df1, aes(x = as.factor(n_procs), y = tp, color="model", group = 1))  + 
    
    labs(x = "mpi procs", y = "Time taken (s)") +
    theme(legend.title = element_blank()) +
    scale_colour_manual(values = c("empirical" = "#a6620f", "model" = "#2bacbd" )) +
    labs(title = "")+
    theme(axis.text = element_text(size = rel(1.2)), axis.title.x = element_text(size = rel(1.5)), axis.title.y = element_text(size = rel(1.2)),
          title = element_text(size = rel(1.5)),legend.text = element_text(size=15) )
  
  #ggsave(paste0( "img/model_", name, ".png"), width = 12, height = 8, dpi = 150)
  
}


model_omp_mpi <- function(df, name) {
  df4 <- df[which(df$n_procs == 4), ]
  
  #serial times
  t_tree <- 0.64 - 0.18
  ts <-  df4[1,]$time_taken
  t_ord <- ts - t_tree # ~ time to read the array * 3 * log(N)
  # with N = 10^7 -> t_ord ~ 0.06 *3*log(10^7)
  
  Ts_1 <- 0.06*3 # at least one sequential read
  
  # then parallel time is parallelizable part divided by the numb of workers
  # + sequential part + (Ts - Ts_1)/n_workers
  n <- nrow(df4)
  tp <- rep(0,n) 
  
  #problem size
  p_size <- df4$problem_size[1]/4
  
  tp[1] <- df4[1,]$time_taken
  
  for (i in 2:n){
    #number of threads
    #nt = df4$n_threads[i]
    #tp[i] <- t_tree/(4 + nt) + (0.06 *3*10^(-7)*p_size*log(p_size,2))/nt  + df4$mpi_times[i]
    
    nt = df4$n_threads[i]
    
    p_size <- (df4$problem_size[1]/df4$n_procs[i])
    tp[i] <- (t_tree/(df4$n_procs[1]*nt)) + (0.06 *3*10^(-7)*p_size*log(p_size,2))/nt + df4$mpi_times[i]
    
  }
  
  minimum <- df4$time_taken[6]
  
  ggplot() +
    geom_line(data = df4, aes(x = as.factor(n_threads), y = time_taken, color="empirical", group=1)) +
    geom_point(data = df4, aes(x = as.factor(n_threads), y = time_taken, color="empirical", group = 1))  + 
    
    geom_line(data = df4, aes(x = as.factor(n_threads), y = tp, color="model", group=1)) +
    geom_point(data = df4, aes(x = as.factor(n_threads), y = tp, color="model", group = 1))  +
    
    geom_line() + geom_point() + 
    geom_line(data = df4, aes(x = n_threads, y = minimum),linetype="dashed") + 
    labs(x = "num threads", y = "Time taken (s)") +
    theme(legend.title = element_blank()) +
    scale_colour_manual(values = c("empirical" = "#a6620f", "model" = "#2bacbd" )) +
    labs(title = "") +
    theme(axis.text = element_text(size = rel(1.2)), axis.title.x = element_text(size = rel(1.5)), axis.title.y = element_text(size = rel(1.2)),
          title = element_text(size = rel(1.5)),legend.text = element_text(size=15) )
  
  #ggsave(paste0( "img/model_", name, ".png"), width = 12, height = 8, dpi = 150)
  
}

###########################
