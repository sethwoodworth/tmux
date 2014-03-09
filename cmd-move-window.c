/* $OpenBSD$ */

/*
 * Copyright (c) 2008 Nicholas Marriott <nicm@users.sourceforge.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/types.h>

#include <stdlib.h>

#include "tmux.h"

/*
 * Move a window.
 */

enum cmd_retval	 cmd_move_window_exec(struct cmd *, struct cmd_q *);
void		 cmd_move_window_prepare(struct cmd *, struct cmd_q *);

const struct cmd_entry cmd_move_window_entry = {
	"move-window", "movew",
	"dkrs:t:", 0, 0,
	"[-dkr] " CMD_SRCDST_WINDOW_USAGE,
	0,
<<<<<<< HEAD
	cmd_move_window_exec
};

const struct cmd_entry cmd_link_window_entry = {
	"link-window", "linkw",
	"dks:t:", 0, 0,
	"[-dk] " CMD_SRCDST_WINDOW_USAGE,
	CMD_PREPARESESSION|CMD_PREPAREWINDOW,
	cmd_move_window_exec,
	cmd_move_window_prepare
};

void
cmd_move_window_prepare(struct cmd *self, struct cmd_q *cmdq)
{
	struct args	*args = self->args;

	if (args_has(args, 'r'))
		cmdq->current_state.s = cmd_find_session(cmdq, args_get(args, 't'), 0);
	else {
		cmdq->current_state.wl = cmd_find_window(cmdq, args_get(args, 's'),
		    &cmdq->current_state.s);
		cmdq->current_state.idx = cmd_find_index(cmdq, args_get(args, 't'),
		    &cmdq->current_state.s2);
	}
}

enum cmd_retval
cmd_move_window_exec(struct cmd *self, struct cmd_q *cmdq)
{
	struct args	*args = self->args;
	struct session	*src, *dst, *s;
	struct winlink	*wl;
	char		*cause;
	int		 idx, kflag, dflag;

	if (args_has(args, 'r')) {
		if ((s = cmdq->current_state.s) == NULL)
			return (CMD_RETURN_ERROR);

		session_renumber_windows(s);
		recalculate_sizes();

		return (CMD_RETURN_NORMAL);
	}
	src = cmdq->current_state.s;
	dst = cmdq->current_state.s2;

	if ((wl = cmdq->current_state.wl) == NULL)
		return (CMD_RETURN_ERROR);
	if ((idx = cmdq->current_state.idx) == -2)
		return (CMD_RETURN_ERROR);

	kflag = args_has(self->args, 'k');
	dflag = args_has(self->args, 'd');
	if (server_link_window(src, wl, dst, idx, kflag, !dflag,
	    &cause) != 0) {
		cmdq_error(cmdq, "can't link window: %s", cause);
		free(cause);
		return (CMD_RETURN_ERROR);
	}
	if (self->entry == &cmd_move_window_entry)
		server_unlink_window(src, wl);
	recalculate_sizes();

	return (CMD_RETURN_NORMAL);
}
