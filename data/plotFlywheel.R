plotFly <- function(log) {
  plot(log$Time,log$Rpm.4, type="l", lwd=2, col="yellow")
  lines(log$Time,log$Target.4, col="red", lwd=1)
  lines(log$Time,log$Power, col="green", lwd=1)
  lines(log$Time,log$D, col="purple", lwd=1)
}