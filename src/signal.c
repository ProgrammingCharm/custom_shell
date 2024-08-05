#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <errno.h>

#include "signal.h"

static void
interrupting_signal_handler(int signo)
{
  /* Its only job is to interrupt system calls--like read()--when
   * certain signals arrive--like Ctrl-C.
   */
}

static struct sigaction ignore_action = {.sa_handler = SIG_IGN},
                        interrupt_action = {.sa_handler =
                                                interrupting_signal_handler},
                        old_sigtstp, old_sigint, old_sigttou;

/* Ignore certain signals.
 * 
 * @returns 0 on succes, -1 on failure
 *
 *
 * The list of signals to ignore:
 *   - SIGTSTP
 *   - SIGINT
 *   - SIGTTOU
 *
 * Should be called immediately on entry to main() 
 *
 * Saves old signal dispositions for a later call to signal_restore()
 */
int
signal_init(void)
{
  /* TODO Initialize signals, store old actions 
   *
   * e.g. sigaction(SIGNUM, &new_handler, &saved_old_handler);
   *
   * */
  struct sigaction new_action;

  /* Initialize new_action as a struct sigaction */
  new_action.sa_handler = SIG_IGN;
  sigemptyset(&new_action.sa_mask);
  new_action.sa_flags = 0;
  /*At first, ignore SIGTSTP, SIGINT, SIGTTOU */
  if (sigaction(SIGTSTP, &new_action, &old_sigtstp) < 0) {
    return -1;
  }
  if (sigaction(SIGINT, &new_action, &old_sigint) < 0) {
    return -1;
  }
  if (sigaction(SIGTTOU, &new_action, &old_sigttou) < 0) {
    return -1;
  }
  /*errno = ENOSYS;*/ /* not implemented */
  return 0;
}

/** enable signal to interrupt blocking syscalls (read/getline, etc) 
 *
 * @returns 0 on succes, -1 on failure
 *
 * does not save old signal disposition
 */
int
signal_enable_interrupt(int sig)
{
  /* TODO set the signal disposition for signal to interrupt  */
  /*struct sigaction new_action;*/
  /* Initialized new_action, a struct variable, to try and interrupt system call */
  /*new_action.sa_handler = interrupting_signal_handler;*/
  /*sigemptyset(&new_action.sa_mask);*/
  /*new_action.sa_flags = 0;*/

  /* Try and st the signal disposition, if a failure occure handle */
  if (sigaction(sig, &interrupt_action, 0) < 0) {
    return -1;
  }
  /*errno = ENOSYS;*/ /* not implemented */
  return 0;
}

/** ignore a signal
 *
 * @returns 0 on success, -1 on failure
 *
 * does not save old signal disposition
 */
int
signal_ignore(int sig)
{
  /* TODO set the signal disposition for signal back to its old state */
  struct sigaction old_action; 
  switch (sig) { /* First try and restore old signal disposition */
    case SIGTSTP:
      old_action = old_sigtstp;
      break;
    case SIGINT:
      old_action = old_sigint;
      break;
    case SIGTTOU:
      old_action = old_sigttou;
      break;
    default:
      errno = EINVAL;
      return -1;
  }
  if (sigaction(sig, &old_action, 0) < 0) {
    return -1;
  }
  /*errno = ENOSYS;*/ /* not implemented */
  return 0;
}

/** Restores signal dispositions to what they were when bigshell was invoked
 *
 * @returns 0 on success, -1 on failure
 *
 */
int
signal_restore(void)
{
  /* TODO restore old actions 
   *
   * e.g. sigaction(SIGNUM, &saved_old_handler, NULL);
   *
   * */
  if (sigaction(SIGTSTP, &old_sigtstp, 0) < 0) {
    return -1;
  }
  if (sigaction(SIGINT, &old_sigint, 0) < 0) {
    return -1;
  }
  if (sigaction(SIGTTOU, &old_sigttou, 0) < 0) {
    return -1;
  }
  /*errno = ENOSYS;*/ /* not implemented */
  return 0;
}
