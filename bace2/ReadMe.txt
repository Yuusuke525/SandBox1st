����΂��
�q���[�}���A�J�f�~�[�Q�[���J���b�W�V��ł��B
�d�������l�ł��B

����A������Ă����������̂�
�u�V�[���Ǘ��v�ɂ��ċ����Ăق�����������ł�

EditScene�ɓ����Ă���class C_EDIT���g�������̂ł����A

SceneManager.cpp
///////////////////////////////////////////////////////////////////

void SceneMgr_Update(IDirect3DDevice9* pD3Device, int WINDOW_WIDTH, int WINDOW_HEIGHT)
{
	switch (Scene)
	{
	case Init_EditScene:
		C_EDIT *edit = new C_EDIT(pD3Device, WINDOW_WIDTH, WINDOW_HEIGHT);
	SceneMgr_ChangeScene(Scene_EditScene);
		break;
	case Scene_EditScene:
		edit->draw();
		break;
	case Exit_EditScene:
		delete edit;
		break;

	}
}
/////////////////////////////////////////////////////////////////////
�����I�ȏ����Ƃ��Ă͂��̂悤�ȗ���ɂ������̂ł������̂悤�ɋL�q���Ă�case����
�錾����ƕʂ�case�Ŏg���Ȃ��悤�Ȃ̂ŔY��ł��܂��B

�O���[�o���֐����g��Ȃ��悤�ɂ��Ă���̂ł�������ł��B

�ǂ̂悤�ɂ���Έ��S�ɑf�����Ǘ��ł���̂ł��傤���H